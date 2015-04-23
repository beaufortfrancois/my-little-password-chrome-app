#include "archive.h"
#include "archive_entry.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/var.h"
#include "ppapi/cpp/var_array.h"
#include "ppapi/cpp/var_array_buffer.h"
#include "ppapi/cpp/var_dictionary.h"

namespace {
  const int kMaxBufferSize = 50 * 1024 * 1024; // 50MB
}

class Instance : public pp::Instance {
 public:
  explicit Instance(PP_Instance instance) : pp::Instance(instance)
  {}
  virtual ~Instance() {}

  virtual void HandleMessage(const pp::Var& var_message) {
    if (!var_message.is_dictionary())
      return;

    pp::VarDictionary dict(var_message);

    std::string filename = dict.Get("filename").AsString();
    std::string password = dict.Get("password").AsString();
    pp::VarArrayBuffer data(dict.Get("data"));

    archive* a = archive_write_new();
    pp::VarArrayBuffer archive_buffer(kMaxBufferSize);
    void *buffer = archive_buffer.Map();
    size_t used = 0;

    int r1 = archive_write_set_format_zip(a);
    int r2 = archive_write_add_filter_none(a);
    int r3 = archive_write_open_memory(a, buffer, kMaxBufferSize, &used);
    //int r4 = archive_write_zip_set_compression_deflate(a);
    //int r0 = archive_write_set_options(a, "zip:encryption=aes256");
    //int r4 = archive_write_set_passphrase(a, password.c_str());

    archive_entry *entry = archive_entry_new();
    archive_entry_copy_pathname(entry, filename.c_str());
    archive_entry_set_mode(entry, AE_IFREG | 0755);
    archive_entry_set_size(entry, data.ByteLength());
    int r5 = archive_write_header(a, entry);
    archive_entry_free(entry);

    int r6 = archive_write_data(a, data.Map(), data.ByteLength());
    archive_write_close(a);
    archive_write_free(a);

    //dict.Set("r0", r0);
    dict.Set("r1", r1);
    dict.Set("r2", r2);
    dict.Set("r3", r3);
    //dict.Set("r4", r4);
    dict.Set("r5", r5);
    dict.Set("r6", r6);
    dict.Set("used", int(used));

    pp::VarArrayBuffer encryptedData(used);
    memcpy(encryptedData.Map(), archive_buffer.Map(), used);

    dict.Set("encryptedData", encryptedData);
    PostMessage(dict);
  }
};

class NaclArchiveModule : public pp::Module {
 public:
  NaclArchiveModule() : pp::Module() {}
  virtual ~NaclArchiveModule() {}

  virtual pp::Instance* CreateInstance(PP_Instance instance) {
    return new Instance(instance);
  }
};

namespace pp {
Module* CreateModule() {
  return new NaclArchiveModule();
}
}  // namespace pp
