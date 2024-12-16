# `gtfo`
`gtfo` runs commands in the background, completely transparently.

## Usage
```sh
$ gtfo google-chrome-stable # This command will exit immediately, opening Chrome in the background such that you can close the terminal safely without closing Chrome. Chrome's output will be redirected to /dev/null, so that your terminal doesn't get polluted.
```

## Compilation & Installation
```
$ make
$ sudo make install
```
