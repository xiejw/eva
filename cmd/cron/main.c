#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "base/defs.h"
#include "cron/expr.h"

// -----------------------------------------------------------------------------
// helper methods prototypes.
// -----------------------------------------------------------------------------

static error_t parseOptions(cron_expr_t *expr, int argc, char **argv);
static error_t parsePosInt(char *str, int *v);

// -----------------------------------------------------------------------------
// implementation.
// -----------------------------------------------------------------------------

// print the number of seconds from `now` to the next time cron expression.
int
main(int argc, char **argv)
{
        cron_expr_t expr;
        cronExprInit(&expr);

        if (parseOptions(&expr, argc, argv)) {
                errDump("failed to parse options");
                errFree();
                exit(1);
        }

        time_t current_time = time(NULL);
        time_t next_time;

        if (cronExprNext(&expr, current_time, &next_time)) exit(1);
        printf("%ld", (long)(next_time - current_time));
        return 0;
}

// -----------------------------------------------------------------------------
// helper methods. implementation.
// -----------------------------------------------------------------------------

static struct option longopts[] = {{"hour", required_argument, NULL, 'h'},
                                   {"minute", required_argument, NULL, 'm'},
                                   {NULL, 0, NULL, 0}};

error_t
parseOptions(cron_expr_t *expr, int argc, char **argv)
{
        int ch;
        int v;
        while ((ch = getopt_long(argc, argv, "h:m:", longopts, NULL)) != -1) {
                switch (ch) {
                case 'm':
                        if (parsePosInt(optarg, &v))
                                return errEmitNote("parsing minute");
                        cronFieldSetSingleValue(&expr->minute, v);
                        break;
                case 'h':
                        if (parsePosInt(optarg, &v))
                                return errEmitNote("parsing hour");
                        cronFieldSetSingleValue(&expr->hour, v);
                        break;
                default:
                        return errNew("error on parsing options.");
                }
        }

        if (argv[optind] != NULL)
                return errNew("remaining options cannot be parsed: %s",
                              argv[optind]);
        return OK;
}

error_t
parsePosInt(char *str, int *v)
{
        if (*str == '\0') return errNew("int argument cannot be empty string");
        char *end_p;
        *v = (int)strtol(str, &end_p, /*base=*/10);
        if (*end_p != '\0')
                return errNew("int argument cannot be parsed: %s", str);
        if (*v < 0) return errNew("require positive int argument: %d", *v);
        return OK;
}
