## Cron

### Usage

    # Print the number of seconds until next time matching X hour Y minute, both
    # the flags `--hour` and `--minute` are optional.

    .debug/cron --hour X --minute Y

Then, it can be used in the script, say a Docker, like


    #!/bin/sh

    while true; do
       sleep `cron --hour X --minute Y`
       echo "Current time is `date`."

       # Your job.
    done

### Docker

See dockerfiels folder for usages or use `xiejw/cron` directly.
