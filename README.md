# `goaway`
`goaway` runs commands in the background, completely transparently.

## Usage
```sh
$ goaway google-chrome-stable
$ 
```
The command above will exit immediately, opening Chrome in the background such that you can close the terminal safely without closing Chrome. Chrome's output will be redirected to /dev/null, so that your terminal doesn't get polluted. No more fiddling with `jobs`, `disown`, `&`, `>`, `>>`, `/dev/null` or `nohup`!

## Compilation & Installation
```
$ make
$ sudo make install
```
