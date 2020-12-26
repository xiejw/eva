# My Core C Library

## Compile

```
make             # debug mode.
make RELEASE=1   # release mode.
make ASAN=1 test # asan test mode.
```

# Binaries

## Cron

See [usage](cmd/cron/cron.1) for details.

    # print the number of seconds until next time matching X hour Y minute
    make cron
    .build/cron --hour X --minute Y

## Sudoku

An interesting example demostrates Dancing Links algorithrm.

    make sudoku
