function onModuleLoad() {
  var module = document.getElementById('module');
  document.getElementById('button').disabled = false;
  document.getElementById('button').addEventListener('click', onButtonClick);
}

function onButtonClick() {
  document.getElementById('poney').hidden = true;
  var password = document.getElementById('password').value;
  if (!password.trim())
    return;

  chrome.fileSystem.chooseEntry({type: 'openFile'}, function(entry) {
    if (chrome.runtime.lastError || !entry)
      return;

    entry.file(function(file) {
      var reader = new FileReader();
      reader.onload = function(event) {
        // Send message to NaCl module when file has been successfully read.
        module.postMessage({
          'filename': entry.name,
          'lastModified': file.lastModified,
          'password': password,
          'binaryData': event.target.result,
        });
      }
      reader.readAsArrayBuffer(file);
    });
  });
}

function onModuleMessage(event) {
  console.debug('onModuleMessage', event);

  var data = event.data;
  var filename = data.filename + '.zip';
  chrome.fileSystem.chooseEntry({type: 'saveFile', suggestedName: filename},
      function(writableEntry) {
    if (chrome.runtime.lastError || !writableEntry)
      return;

    writableEntry.createWriter(function(writer) {
      // Write encrypted data on disk.
      var blob = new Blob([data.binaryData]);
      writer.seek(0);
      writer.write(blob);
      writer.onwriteend = function() {
        writer.onwriteend = null;
        writer.truncate(blob.size);
        // Reveal poney on write.
        document.getElementById('poney').hidden = false;
      }
    });
  });
}

window.addEventListener('DOMContentLoaded', function() {
  var embed = document.createElement('embed');
  embed.setAttribute('id', 'module');
  embed.setAttribute('width', 0);
  embed.setAttribute('height', 0);
  embed.setAttribute('type', 'application/x-pnacl');
  embed.setAttribute('src', 'pnacl/Release/module.nmf');

  var listenerDiv = document.getElementById('listener');
  listenerDiv.addEventListener('load', onModuleLoad, true);
  listenerDiv.addEventListener('message', onModuleMessage, true);
  listenerDiv.appendChild(embed);
});
