#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/var.h"
#include "ppapi/cpp/var_array.h"
#include "ppapi/cpp/var_array_buffer.h"
#include "ppapi/cpp/var_dictionary.h"

#include "archive.h"
#include "archive_entry.h"

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
    pp::VarArrayBuffer array_buffer(dict.Get("data"));

    struct archive* a = archive_write_new();
    struct archive_entry *entry;

    a = archive_write_new();
    //archive_write_add_filter_gzip(a);
    //archive_write_set_format_pax_restricted(a);
    //archive_write_set_passphrase(a, password);

    //archive_write_open_filename(a, filename, 10240);

    entry = archive_entry_new();
    //archive_entry_set_pathname(entry, filename);
    //archive_entry_set_size(entry, sizeof(data));
    //archive_entry_set_filetype(entry, AE_IFREG);
    //archive_entry_set_perm(entry, 0644);
    //archive_write_header(a, entry);
    //archive_write_data(a, data, sizeof(data));
    //archive_entry_free(entry);

    archive_write_close(a);
    archive_write_free(a);

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
