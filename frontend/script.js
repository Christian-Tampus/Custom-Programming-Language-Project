/* UPDATE VERSION [5] */

/*
==================================================
Program Start
==================================================
*/
console.log("[CLIENT] script.js Program Start!");

/*
==================================================
Global Constants
==================================================
*/
const textEditorPlaceholder = "Your Code Here [LATER ADD BOILER PLATE TEST CODE]";
const html = document.documentElement;
const textEditorDiv = document.getElementById("text-editor-div");
const textEditor = document.getElementById("text-editor");
const lineNumbers = document.getElementById("line-numbers");
const lightThemeButton = document.getElementById("light-theme-button");
const darkThemeButton = document.getElementById("dark-theme-button");
const terminalDiv = document.getElementById("terminal-div");
const terminal = document.getElementById("terminal");
const lightThemeColor1 = "#333333";
const lightThemeColor2 = "#F5F5F5";
const darkThemeColor1 = "lime";
const darkThemeColor2 = "black";

/*
==================================================
Functions
==================================================
*/
function updateLineNumbers() {
    let lines = textEditor.value.split("\n").length;
    let lineNumbersNewInnerText = "";
    for (let line = 0; line < lines; line++) {
        lineNumbersNewInnerText += (line + 1).toString() + "\n";
    };
    lineNumbers.innerText = lineNumbersNewInnerText;
};
function syncScroll() {
    lineNumbers.scrollTop = textEditor.scrollTop;
};
function lightTheme() {
    html.style.backgroundColor = lightThemeColor2;
    textEditorDiv.style.border = "3px solid " + lightThemeColor1;
    textEditorDiv.style.backgroundColor = lightThemeColor2;
    lineNumbers.style.color = lightThemeColor1;
    lineNumbers.style.backgroundColor = lightThemeColor2;
    lineNumbers.style.border = "1px solid " + lightThemeColor1;
    textEditor.style.backgroundColor = lightThemeColor2;
    textEditor.style.color = lightThemeColor1;
    textEditor.style.border = "1px solid " + lightThemeColor1;
    textEditor.style.setProperty("--text-editor-placeholder-color", lightThemeColor1);
    textEditor.style.setProperty("--text-editor-placeholder-opacity", 1);
    terminal.style.setProperty("--terminal-placeholder-color", lightThemeColor1);
    terminal.style.setProperty("--terminal-placeholder-opacity", 1);
    terminalDiv.style.backgroundColor = lightThemeColor2;
    terminalDiv.style.border = "3px solid " + lightThemeColor1;
    terminal.style.backgroundColor = lightThemeColor2;
    terminal.style.border = "1px solid " + lightThemeColor1;
}
function darkTheme() {
    html.style.backgroundColor = darkThemeColor2;
    textEditorDiv.style.border = "3px solid " + darkThemeColor1;
    textEditorDiv.style.backgroundColor = darkThemeColor2;
    lineNumbers.style.color = darkThemeColor1;
    lineNumbers.style.backgroundColor = darkThemeColor2;
    lineNumbers.style.border = "1px solid " + darkThemeColor1;
    textEditor.style.backgroundColor = darkThemeColor2;
    textEditor.style.color = darkThemeColor1;
    textEditor.style.border = "1px solid " + darkThemeColor1;
    textEditor.style.setProperty("--text-editor-placeholder-color", darkThemeColor1);
    textEditor.style.setProperty("--text-editor-placeholder-opacity", 1);
    terminal.style.setProperty("--terminal-placeholder-color", darkThemeColor1);
    terminal.style.setProperty("--terminal-placeholder-opacity", 1);
    terminalDiv.style.backgroundColor = darkThemeColor2;
    terminalDiv.style.border = "3px solid " + darkThemeColor1;
    terminal.style.backgroundColor = darkThemeColor2;
    terminal.style.border = "1px solid " + darkThemeColor1;
}

/*
==================================================
Event Listeners
==================================================
*/
textEditor.addEventListener("input", updateLineNumbers);
textEditor.addEventListener("scroll", syncScroll);
lightThemeButton.addEventListener("click", lightTheme);
darkThemeButton.addEventListener("click", darkTheme);

/*
==================================================
Initializers
==================================================
*/
textEditor.placeholder = textEditorPlaceholder;
updateLineNumbers();

/*
==================================================
Program End
==================================================
*/
console.log("[CLIENT] script.js Program Terminated...");