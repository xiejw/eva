#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#include <wordexp.h>

constexpr const char* kEvaPath = "~/Workspace/eva";
constexpr const char* kDependencyFolder = "dependencies";
constexpr const char* kEvaPathEnvVarName = "EVA_PATH";

namespace {

/// Reports the fatal error to `stderr` and exits immediately.
void FatalError(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);
  fprintf(stderr, "\n");
  exit(1);
}

/// Expands the `path` according to the shell-style.
std::string ExpandPath(const std::string& path) {
  wordexp_t exp_result;
  if (wordexp(path.c_str(), &exp_result, 0)) {
    FatalError("Failed to expand the path: %s", path.c_str());
  }

  // The simpliest case: one output.
  if (exp_result.we_wordc == 1) {
    std::string result{exp_result.we_wordv[0]};
    wordfree(&exp_result);
    return result;
  }

  // For multiple outputs, concat them.
  std::stringstream result;
  for (uint i = 0; i < exp_result.we_wordc; ++i) {
    if (i != 0) result << " ";
    result << exp_result.we_wordv[i];
  }
  wordfree(&exp_result);
  return result.str();
}

}  // namespace

namespace Formula {

struct Formula {
  const char* name;
  std::string cxxflags;
  std::string ldflags;

  Formula(const char* a_name, const char* a_cxxflags, const char* a_ldflags)
      : name{a_name},
        cxxflags{ExpandPath(a_cxxflags)},
        ldflags{ExpandPath(a_ldflags)} {}
};

struct OutputInfo {
  bool print_cxxflags;
  bool print_ldflags;
  Formula* formula;
};

class Outputs {
 public:
  Outputs() : formulars_{}, infos_{} {
    // Initialze the information table. Right now, we just hard code them.
    formulars_.push_back(std::make_unique<Formula>(
        /*name=*/"gflags",
        /*cxxflags=*/"-I${EVA_PATH}/gflags/buildlib/include/",
        /*ldflags=*/"${EVA_PATH}/gflags/buildlib/lib/libgflags_nothreads.a"));

    formulars_.push_back(std::make_unique<Formula>(
        /*name=*/"googletest",
        /*cxxflags=*/"-I${EVA_PATH}/googletest/googletest/include/",
        /*ldflags=*/
        "${EVA_PATH}/googletest/buildlib/lib/libgtest.a -lpthread"));
  }

 public:
  void RegisterCXXFlags(const char* name) {
    for (auto& formula : formulars_) {
      // Tries to find a match.
      if (strcmp(formula->name, name) == 0) {
        auto info = std::make_unique<OutputInfo>();
        info->print_cxxflags = true;
        info->formula = formula.get();
        infos_.push_back(std::move(info));
        return;
      }
    }
    FatalError("Failed to find library with name %s", name);
  };

  void RegisterLDFlags(const char* name) {
    for (auto& formula : formulars_) {
      // Tries to find a match.
      if (strcmp(formula->name, name) == 0) {
        auto info = std::make_unique<OutputInfo>();
        info->print_ldflags = true;
        info->formula = formula.get();
        infos_.push_back(std::move(info));
        return;
      }
    }
    FatalError("Failed to find library with name %s", name);
  };

  void Print() {
    bool first_one = true;

    // First pass: Print cxxflags
    for (auto& info : infos_) {
      if (!info->print_cxxflags) continue;
      if (first_one)
        first_one = false;
      else
        std::cout << " ";
      std::cout << info->formula->cxxflags;
    }

    // Second pass: Print ldflags
    for (auto& info : infos_) {
      if (!info->print_ldflags) continue;
      if (first_one)
        first_one = false;
      else
        std::cout << " ";
      std::cout << info->formula->ldflags;
    }
  }

 private:
  std::vector<std::unique_ptr<Formula>> formulars_;
  std::vector<std::unique_ptr<OutputInfo>> infos_;
};

}  // namespace Formula

namespace Parser {

class Parser {
 public:
  Parser(int argc, char** argv) : argc_(argc), argv_(argv){};

  std::unique_ptr<Formula::Outputs> Parse() {
    auto outputs = std::make_unique<Formula::Outputs>();

    // Nothing parsed.
    if (argc_ == 1) return outputs;

    int current_index = 1;
    while (current_index < argc_) {
      if (strcmp(argv_[current_index], "--cxxflags") == 0) {
        ParseCXXFLags(outputs.get(), &current_index);
        continue;
      }

      if (strcmp(argv_[current_index], "--ldflags") == 0) {
        ParseLDFLags(outputs.get(), &current_index);
        continue;
      }

      FatalError("Failed to parse the flag %s", argv_[current_index]);
    }

    return outputs;
  }

 private:
  void ParseCXXFLags(Formula::Outputs* outputs, int* current_index) {
    while (true) {
      ++*current_index;
      // End of the tokens. So treat as empty arguments for --cxxflags.
      if (*current_index == argc_) return;
      // End of expected tokens.
      if (argv_[*current_index][0] == '-') return;

      outputs->RegisterCXXFlags(argv_[*current_index]);
    }
  };

  void ParseLDFLags(Formula::Outputs* outputs, int* current_index) {
    while (true) {
      ++*current_index;
      // End of the tokens. So treat as empty arguments for --cxxflags.
      if (*current_index == argc_) return;
      // End of expected tokens.
      if (argv_[*current_index][0] == '-') return;

      outputs->RegisterLDFlags(argv_[*current_index]);
    }
  };

 private:
  int argc_;
  char** argv_;
};
}  // namespace Parser

namespace {

void PrepareEnvironmentVariableForEvaPath() {
  // EVA_PATH=${kEvaPath}/${kDependencyFolder}
  std::string full_eval_path = ExpandPath(kEvaPath) + "/" + kDependencyFolder;
  setenv(kEvaPathEnvVarName, full_eval_path.c_str(), 1);
}

}  // namespace

int main(int argc, char** argv) {
  PrepareEnvironmentVariableForEvaPath();
  Parser::Parser parser{argc, argv};
  parser.Parse()->Print();
  return 0;
}
