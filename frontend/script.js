/* UPDATE VERSION [4] */

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
const textEditorDiv = document.getElementById("text-editor-div");
const textEditor = document.getElementById("text-editor");
const lineNumbers = document.getElementById("line-numbers");
const lightThemeButton = document.getElementById("light-theme-button");
const darkThemeButton = document.getElementById("dark-theme-button");

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
    textEditorDiv.style.border = "3px solid #333333";
    textEditorDiv.style.backgroundColor = "#F5F5F5";
    lineNumbers.style.color = "#333333";
    lineNumbers.style.backgroundColor = "#F5F5F5";
    lineNumbers.style.border = "1px solid #333333";
    textEditor.style.backgroundColor = "#F5F5F5";
    textEditor.style.color = "#333333";
    textEditor.style.border = "1px solid #333333";
    textEditor.style.setProperty("--placeholder-color", "#333333");
    textEditor.style.setProperty("--placeholder-opacity", 1);
}
function darkTheme() {
    textEditorDiv.style.border = "3px solid #D4D4D4";
    textEditorDiv.style.backgroundColor = "#1E1E1E";
    lineNumbers.style.color = "#D4D4D4";
    lineNumbers.style.backgroundColor = "#1E1E1E";
    lineNumbers.style.border = "1px solid #D4D4D4";
    textEditor.style.backgroundColor = "#1E1E1E";
    textEditor.style.color = "#D4D4D4";
    textEditor.style.border = "1px solid #D4D4D4";
    textEditor.style.setProperty("--placeholder-color", "#D4D4D4");
    textEditor.style.setProperty("--placeholder-opacity", 1);
}

/*
==================================================
Event Listeners
==================================================
*/
textEditor.addEventListener("input", updateLineNumbers);
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