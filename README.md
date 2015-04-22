WIP

## Building

### Requirements:
- NaCl SDK https://developer.chrome.com/native-client/sdk/download
- naclports https://code.google.com/p/naclports/

```bash
export NACL_SDK_ROOT=/path/to/nacl_sdk/pepper_XX
/path/to/naclports/src/bin/naclports  --toolchain=pnacl install libarchive
```

### Get the code:
```bash
git clone git@github.com:beaufortfrancois/my-little-password.git
cd my-little-password/src
make
```

