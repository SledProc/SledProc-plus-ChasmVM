window.onload = function WindowLoad(event) {
  window.webChannel = new QWebChannel(qt.webChannelTransport, function (
    channel
  ) {
    //  var cpp = channel.objects.JsInterface;
    //  cpp.log("Hello from JavaScript!!!");
  });
};
