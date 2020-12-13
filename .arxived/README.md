# My Core C Library

## Compile

```
# debug mode.
make

# release mode.
make RELEASE=1
```

# Binaries

## Cron

TL;DR;

    # Print the number of seconds until next time matching X hour Y minute
    .debug/cron --hour X --minute Y

See [usage](docs/cron.md) for details.

## Shasum

TL;DR;

    # Prints the shasum 256 checksum for a file.
    .debug/shasum <file_name>

## Scanner

TL;DR;

    # Prints the shasum 256 checksum of each non-hidden file under a directory
    # tree (a file in hidden folder is considered as hidden file.)
    .debug/scanner --dir <dire_name>

