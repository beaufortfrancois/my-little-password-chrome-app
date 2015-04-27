# My Little Password Chrome App

This Chrome App lets you create password protected zip files thanks to [Portable Native
Client](http://gonacl.com) and more specifically
[libarchive](https://github.com/libarchive/libarchive) NaCl port.

Get it on the Chrome Web Store at https://chrome.google.com/webstore/detail/TODO

<img src="https://raw.githubusercontent.com/beaufortfrancois/my-little-password-chrome-app/master/screenshot.png">

## Image Credits

http://www.deviantart.com/art/Group-Background-5-Water-Reservoir-306196848

## Development Instructions

### Requirements

- [Download the Native Client SDK](https://developer.chrome.com/native-client/sdk/download)
- [Check out NaCl Ports](https://code.google.com/p/naclports/wiki/HowTo_Checkout)

### Setup

```bash
# Download pepper_42 bundle.
/path/to/nacl_sdk/naclsdk update pepper_42

# Set NACL_SDK_ROOT environment variable to the pepper_42 bundle directory.
export NACL_SDK_ROOT=/path/to/nacl_sdk/pepper_42

# Check out NaCl Ports pepper_42 branch.
cd /path/to/naclports/src
git checkout -b pepper_42 origin/pepper_42

# Clone "My Little Password" Git repository
cd /path/to/workspace/
git clone git@github.com:beaufortfrancois/my-little-password-chrome-app.git
cd my-little-password-chrome-app

# Install libarchive-fork custom NaCl Port.
/path/to/naclports/src/bin/naclports --toolchain=pnacl \
install src/third-party/libarchive-fork
```

### Build

```bash
# Build Release NaCl module
cd /path/to/my-little-password/src
make
```
OR
```bash
# Build Debug NaCl module
cd /path/to/my-little-password/src
CONFIG=Debug make
```