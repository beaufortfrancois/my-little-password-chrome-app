WIP

### Requirements
- [NaCl SDK](https://developer.chrome.com/native-client/sdk/download)
- [NaCl ports](https://code.google.com/p/naclports)

### Build
```bash
# Download the pepper_42 bundle
/path/to/nacl_sdk update pepper_42
# Set the NACL_SDK_ROOT environment variable to the pepper_42 bundle directory.
export NACL_SDK_ROOT=/path/to/nacl_sdk/pepper_42

# Check out pepper_42 branch for NaCl ports.
cd /path/to/naclports/src
git checkout -b pepper_42 origin/pepper_42

# Clone Git Repo
cd /path/to/workspace/
git clone git@github.com:beaufortfrancois/my-little-password.git

# Install libarchive NaCl port.
cd my-little-password/src/third-party
/path/to/naclports/src/bin/naclports --toolchain=pnacl install libarchive-fork

# Build NaCl module
cd my-little-password/src
make
```

### Credits
http://www.deviantart.com/art/Group-Background-5-Water-Reservoir-306196848
