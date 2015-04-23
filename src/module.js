function onModuleLoad() {
  var module = document.getElementById('module');
  document.getElementById('button').disabled = false;
  document.getElementById('button').addEventListener('click', onButtonClick);
}

function onButtonClick() {
  chrome.fileSystem.chooseEntry({type: 'openFile'}, function(entry) {
    entry.file(function(file) {
      var reader = new FileReader();
      reader.onload = function(event) {
        module.postMessage({
          'filename': entry.name,
          'password': document.getElementById('password').value.trim(),
          'data': event.target.result,
        });
      }
      reader.readAsArrayBuffer(file);
    });
  });
}

function onModuleMessage(event) {
  document.getElementById('log').textContent = JSON.stringify(event.data, null, 2);
  var filename = event.data.filename + '.zip';
  chrome.fileSystem.chooseEntry({type: 'saveFile', suggestedName: filename},
      function(writableEntry) {
    if (!writableEntry) {
      return;
    }
    writableEntry.createWriter(function(writer) {
      var blob = new Blob([event.data.encryptedData], {type : 'application/zip'});
      writer.seek(0);
      writer.write(blob);
      writer.onwriteend = function() {
        writer.onwriteend = null;
        writer.truncate(blob.size);
        console.log('Written on', writableEntry.name);
      }
    });
  });
}

window.addEventListener('DOMContentLoaded', function() {
  var embed = document.createElement('embed');
  embed.setAttribute('id', 'module');
  embed.setAttribute('width', 0);
  embed.setAttribute('height', 0);
  embed.setAttribute('src', 'pnacl/Release/module.nmf');
  embed.setAttribute('type', 'application/x-pnacl');

  var listenerDiv = document.getElementById('listener');
  listenerDiv.addEventListener('load', onModuleLoad, true);
  listenerDiv.addEventListener('message', onModuleMessage, true);
  listenerDiv.appendChild(embed);
});
