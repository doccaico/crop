This is C version of https://gitlab.com/-/snippets/2027453

I've only tested it on Linux.

### Build

```
# build
$ make

# install to ~/bin (if you change it, edit Makefile)
$ make install

# uninstall
$ make uninstall
```

### Prerequirement

#### Used
- 1. import, display, python3(using `requests`)
- 2. libX11.a

#### Upload an image to Imgur.com
- 1. save your Client ID in ~/.imgur
- 2. (install) pip install  requests

### Usage

#### Keybinding

```
'1': set a (x, y) top-left-pos
'2': set a (x, y) bottom-right-pos
'3': take an image(~/Desktop/FILENAME)
'v': view an image(~/Desktop/FILENAME)
'i': upload an image(~/Desktop/FILENAME) to Imgur.com
'ESC': exit
```
