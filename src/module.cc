#include <ppapi/cpp/instance.h>
#include <ppapi/cpp/module.h>
#include <ppapi/cpp/var_array_buffer.h>
#include <ppapi/cpp/var_dictionary.h>
#include <time.h>

#include "archive.h"
#include "archive_entry.h"

namespace {
  const int kMaxBufferSize = 512 * 1024 * 1024; // 512MB
}

class Instance : public pp::Instance {
 public:
  explicit Instance(PP_Instance instance) : pp::Instance(instance) {}
  virtual ~Instance() {}

  virtual void HandleMessage(const pp::Var& var_message) {
    if (!var_message.is_dictionary())
      return;

    pp::VarDictionary dict(var_message);
    std::string filename = dict.Get("filename").AsString();
    std::string password = dict.Get("password").AsString();
    time_t lastModified = dict.Get("lastModified").AsDouble() / 1000;
    pp::VarArrayBuffer data(dict.Get("binaryData"));

    void *buffer = malloc(kMaxBufferSize);
    size_t used = 0;

    archive* a = archive_write_new();
    archive_write_set_format_zip(a);
    archive_write_add_filter_none(a);
    archive_write_set_options(a, "zip:encryption=aes256");
    archive_write_set_passphrase(a, password.c_str());
    archive_write_open_memory(a, buffer, kMaxBufferSize, &used);

    archive_entry *entry = archive_entry_new();
    archive_entry_copy_pathname(entry, filename.c_str());
    archive_entry_set_mode(entry, AE_IFREG | 0755);
    archive_entry_set_mtime(entry, lastModified, 0);
    archive_entry_set_size(entry, data.ByteLength());
    archive_write_header(a, entry);
    archive_entry_free(entry);

    archive_write_data(a, data.Map(), data.ByteLength());
    archive_write_close(a);
    archive_write_free(a);

    pp::VarArrayBuffer encryptedData(used);
    memcpy(encryptedData.Map(), buffer, used);
    free(buffer);

    // Replace original data with encrypted data.
    dict.Set("binaryData", encryptedData);
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
}
