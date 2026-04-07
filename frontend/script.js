/* UPDATE VERSION [7] */

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
const terminalPlaceholder = "[SYSTEM MESSAGE] Standard Output!";
const terminalInputPlaceholder = "[SYSTEM MESSAGE] Standard Input!";
const html = document.documentElement;
const textEditorDiv = document.getElementById("text-editor-div");
const textEditor = document.getElementById("text-editor");
const lineNumbers = document.getElementById("line-numbers");
const lightThemeButton = document.getElementById("light-theme-button");
const darkThemeButton = document.getElementById("dark-theme-button");
const terminalDiv = document.getElementById("terminal-div");
const terminal = document.getElementById("terminal");
const terminalInputDiv = document.getElementById("terminal-input-div");
const terminalInput = document.getElementById("terminal-input");
const documentationDiv = document.getElementById("documentation-div");
const documentation = document.getElementById("documentation");
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
    /*
    FIX THE BORDER RIGHT OF #line-numbers SO THAT IT ITS border-right: none;
    */
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
    terminalInput.style.setProperty("--terminal-input-placeholder-color", lightThemeColor1);
    terminalInput.style.setProperty("--terminal-input-placeholder-opacity", 1);
    documentation.style.setProperty("--documentation-placeholder-color", lightThemeColor1);
    documentation.style.setProperty("--documentation-placeholder-opacity", 1);
    terminalDiv.style.backgroundColor = lightThemeColor2;
    terminalDiv.style.border = "3px solid " + lightThemeColor1;
    terminal.style.backgroundColor = lightThemeColor2;
    terminal.style.border = "1px solid " + lightThemeColor1;
    terminalInputDiv.style.backgroundColor = lightThemeColor2;
    terminalInputDiv.style.border = "3px solid " + lightThemeColor1;
    terminalInput.style.backgroundColor = lightThemeColor2;
    terminalInput.style.border = "1px solid " + lightThemeColor1;
    terminalInput.style.color = lightThemeColor1;
    documentationDiv.style.backgroundColor = lightThemeColor2;
    documentationDiv.style.border = "3px solid " + lightThemeColor1;
    documentation.style.backgroundColor = lightThemeColor2;
    documentation.style.border = "1px solid " + lightThemeColor1;
    documentation.style.color = lightThemeColor1;
};
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
    terminalInput.style.setProperty("--terminal-input-placeholder-color", darkThemeColor1);
    terminalInput.style.setProperty("--terminal-input-placeholder-opacity", 1);
    documentation.style.setProperty("--documentation-placeholder-color", darkThemeColor1);
    documentation.style.setProperty("--documentation-placeholder-opacity", 1);
    terminalDiv.style.backgroundColor = darkThemeColor2;
    terminalDiv.style.border = "3px solid " + darkThemeColor1;
    terminal.style.backgroundColor = darkThemeColor2;
    terminal.style.border = "1px solid " + darkThemeColor1;
    terminalInputDiv.style.backgroundColor = darkThemeColor2;
    terminalInputDiv.style.border = "3px solid " + darkThemeColor1;
    terminalInput.style.backgroundColor = darkThemeColor2;
    terminalInput.style.border = "1px solid " + darkThemeColor1;
    terminalInput.style.color = darkThemeColor1;
    documentationDiv.style.backgroundColor = darkThemeColor2;
    documentationDiv.style.border = "3px solid " + darkThemeColor1;
    documentation.style.backgroundColor = darkThemeColor2;
    documentation.style.border = "1px solid " + darkThemeColor1;
    terminalInput.style.color = darkThemeColor1;
};
function getInputValue(event) {
    if (event.key == "Enter") {
        const inputElement = event.target;
        const inputValue = inputElement.value;
        console.log("[CLIENT] inputElement.id: ", inputElement.id, "; Input Value: ", inputValue);
        inputElement.value = "";
    };
};
function codeDocumentation() {
    let maxLines = 20;
    let documentationText = "";
    for (let line = 0; line < maxLines; line++) {
        documentationText += (line + 1 >= maxLines ? "[CODE DOCUMENTATION]" : "[CODE DOCUMENTATION]\n");
    };
    return documentationText;
};

/*
==================================================
Event Listeners
==================================================
*/
textEditor.addEventListener("input", updateLineNumbers);
textEditor.addEventListener("scroll", syncScroll);
lightThemeButton.addEventListener("click", lightTheme);
darkThemeButton.addEventListener("click", darkTheme);
terminalInput.addEventListener("keydown", getInputValue);

/*
==================================================
Initializers
==================================================
*/
textEditor.placeholder = textEditorPlaceholder;
terminal.placeholder = terminalPlaceholder;
terminalInput.placeholder = terminalInputPlaceholder;
documentation.placeholder = codeDocumentation();
documentation.value = codeDocumentation();
updateLineNumbers();

/*
==================================================
Program End
==================================================
*/
console.log("[CLIENT] script.js Program Terminated...");