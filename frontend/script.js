/* UPDATE VERSION [46] */

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
const PORT = 8080;
const textEditorPlaceholder = "Your Code Here [LATER ADD BOILER PLATE TEST CODE]";
const terminalPlaceholder = "[SYSTEM MESSAGE] Standard Output!";
const terminalInputPlaceholder = "[SYSTEM MESSAGE] Standard Input!";
const html = document.documentElement;
const textEditorDiv = document.getElementById("text-editor-div");
const textEditor = document.getElementById("text-editor");
const lineNumbers = document.getElementById("line-numbers");
const lightThemeButton = document.getElementById("light-theme-button");
const darkThemeButton = document.getElementById("dark-theme-button");
const runCodeButton = document.getElementById("run-code-button");
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
const standardPlaceholderColorVariable = "--standard-placeholder-color";
const standardPlaceholderOpacityVariable = "--standard-placeholder-opacity";

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
function addTabToTextEditor(event) {
    if (event.key == "Tab") {
        event.preventDefault(); //JS Method To Stop Browser's Default Behavior
        let tabSpace = "    ";
        let selectionStart = this.selectionStart; //Text Selection/Highlighted Start
        let selectionEnd = this.selectionEnd; //Text Selection/Highlighted End
        /*
        Example:
        Hello [world]
               ↑    ↑
        012345 6....11
        selectionStart = 6 (Index 6)
        selectionEnd = 11 (Index 11)
        */
        //Insert The Tab Character
        this.value = this.value.substring(0, selectionStart) + tabSpace + this.value.substring(selectionEnd);
        //Move Cursor After Tab
        this.selectionEnd = start + 1;
        this.selectionStart = this.selectionEnd;
    };
};
function lightTheme() {
    html.style.backgroundColor = lightThemeColor2;
    textEditorDiv.style.border = "3px solid " + lightThemeColor1;
    textEditorDiv.style.backgroundColor = lightThemeColor2;
    lineNumbers.style.color = lightThemeColor1;
    lineNumbers.style.backgroundColor = lightThemeColor2;
    lineNumbers.style.borderTop = "1px solid " + lightThemeColor1;
    lineNumbers.style.borderLeft = "1px solid " + lightThemeColor1;
    lineNumbers.style.borderBottom = "1px solid " + lightThemeColor1;
    lineNumbers.style.borderRight = "none";
    textEditor.style.backgroundColor = lightThemeColor2;
    textEditor.style.color = lightThemeColor1;
    textEditor.style.border = "1px solid " + lightThemeColor1;
    textEditor.style.setProperty(standardPlaceholderColorVariable, lightThemeColor1);
    textEditor.style.setProperty(standardPlaceholderOpacityVariable, 1);
    terminal.style.color = lightThemeColor1;
    terminal.style.setProperty(standardPlaceholderColorVariable, lightThemeColor1);
    terminal.style.setProperty(standardPlaceholderOpacityVariable, 1);
    terminalInput.style.setProperty(standardPlaceholderColorVariable, lightThemeColor1);
    terminalInput.style.setProperty(standardPlaceholderOpacityVariable, 1);
    documentation.style.setProperty(standardPlaceholderColorVariable, lightThemeColor1);
    documentation.style.setProperty(standardPlaceholderOpacityVariable, 1);
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
    lineNumbers.style.borderTop = "1px solid " + darkThemeColor1;
    lineNumbers.style.borderLeft = "1px solid " + darkThemeColor1;
    lineNumbers.style.borderBottom = "1px solid " + darkThemeColor1;
    lineNumbers.style.borderRight = "none";
    textEditor.style.backgroundColor = darkThemeColor2;
    textEditor.style.color = darkThemeColor1;
    textEditor.style.border = "1px solid " + darkThemeColor1;
    textEditor.style.setProperty(standardPlaceholderColorVariable, darkThemeColor1);
    textEditor.style.setProperty(standardPlaceholderOpacityVariable, 1);
    terminal.style.color = darkThemeColor1;
    terminal.style.setProperty(standardPlaceholderColorVariable, darkThemeColor1);
    terminal.style.setProperty(standardPlaceholderOpacityVariable, 1);
    terminalInput.style.setProperty(standardPlaceholderColorVariable, darkThemeColor1);
    terminalInput.style.setProperty(standardPlaceholderOpacityVariable, 1);
    documentation.style.setProperty(standardPlaceholderColorVariable, darkThemeColor1);
    documentation.style.setProperty(standardPlaceholderOpacityVariable, 1);
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
    documentation.style.color = darkThemeColor1;
};
function codeDocumentation() {
    let maxLines = 20;
    let documentationText = "";
    for (let line = 0; line < maxLines; line++) {
        documentationText += (line + 1 >= maxLines ? "[CODE DOCUMENTATION]" : "[CODE DOCUMENTATION]\n");
    };
    return documentationText;
};
async function executeCode() {
    let clientCode = textEditor.value;
    let clientInput = terminalInput.value;
    console.log("[CLIENT] Execute Code! code:\n", clientCode);
    try {
        const result = await fetch("http://localhost:" + PORT.toString() + "/execute", {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify({
                clientCode: clientCode,
                clientInput: clientInput,
            })
        });
        const data = await result.json();
        if (data.output != null) {
            terminal.value = data.output;
        };
        console.log("[CLIENT] executeCode() Response From Server At Port {" + PORT.toString() + "} Data:", data);
    } catch (error) {
        console.log("[CLIENT] executeCode() Request Failed:", error);
    };
};

/*
==================================================
Event Listeners
==================================================
*/
textEditor.addEventListener("input", updateLineNumbers);
textEditor.addEventListener("scroll", syncScroll);
textEditor.addEventListener("keydown", addTabToTextEditor);
lightThemeButton.addEventListener("click", lightTheme);
darkThemeButton.addEventListener("click", darkTheme);
runCodeButton.addEventListener("click", executeCode);

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