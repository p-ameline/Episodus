// Standard functions used by Doc-O-Matic generated
// HTML documentation.
// This functions may be modified and distributed freely.

// This function opens the file Filename
// in Window or Frame TargetWindow.
  function fillframe(TargetWindow, Filename) {
  // not implemented yet
}

// This function opens the (relative) URL
// in window PopupWindowName at screen
// coordinates X and Y.
// Popups are not implemented in this version.
function showpopup(Target, PopupWindowName, X, Y) {
  window.open(Target, window.name, "");
}

// This function sets the window name of a window
// to the specified value. It is used to initialize
// the window name when a document is loaded.
function setupwindow(WindowName) {
  window.name = WindowName;
}

