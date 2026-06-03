/* UPDATE VERSION [FINAL] */

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

/*
==================================================
Updates Line Numbers
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

/*
==================================================
Syncs Scroll Bar
==================================================
*/
function syncScroll() {
    lineNumbers.scrollTop = textEditor.scrollTop;
};

/*
==================================================
Adds Tab To The Code Text Editor Upon Pressing The Tab Key
==================================================
*/
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
        this.selectionEnd = selectionStart + tabSpace.length;
        this.selectionStart = this.selectionEnd;
    };
};

/*
==================================================
Changes Editor, Terminal, Input Terminal & Code Documentation Theme To Light
==================================================
*/
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

/*
==================================================
Changes Editor, Terminal, Input Terminal & Code Documentation Theme To Dark
==================================================
*/
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

/*
==================================================
Updates The Code Documentation Text Value Content
==================================================
*/
function codeDocumentation() {
    let codeDocumentationArray = [
        "Custom Programming Language Documentation",
        "Developed By: Christian Tampus",
        "",
        "==================================================",
        "",
        "1. Comments: The Language Supports Only Single Line Comments, All Single Line Comments Must Start With '!'.",
        "Single Line Comments Example:",
        "!This Is A Single Line Comment",
        "",
        "==================================================",
        "",
        "2. Variables: The Language Supports Only 5 Variable Types: INTEGER, DECIMAL, CHARACTER, BOOLEAN, STRING. Variable Names Must Contain Only Alphanumerical [A-Z,a-z,0-9] Or Underscores '_'. Variable Names Must Start With Either Alphabetical Characters Or Underscores.",
        "Variable Example:",
        "VARIABLE_TYPE VARIABLE_NAME; => INTEGER _int1; OR DECIMAL dec1;",
        "VARIABLE_TYPE VARIABLE_NAME = VALID_DATA_TYPE; => CHARACTER _char1 = 'A'; OR BOOLEAN bool1 = TRUE; OR STRING _str1 = \"HELLO WORLD\";",
        "",
        "==================================================",
        "",
        "3. Constants: Similar To Variables Only That They Cannot Be Overwritten And Must Be Declared & Assigned At The Same Time And Must Contain The 'CONSTANT' Keyword.",
        "Constant Example:",
        "CONSTANT VARIABLE_TYPE VARIABLE_NAME = VALID_DATA_TYPE; => CONSTANT INTEGER _INT1 = 100;",
        "",
        "==================================================",
        "",
        "4. Standard I/O: The Language Supports Standard Input & Output. Requires An Input Buffer To Pre-Fill For Any Expected Input Function Calls In The Program.",
        "Standard I/O Example:",
        "INTEGER age;",
        "output(\"Please Enter Your Age\");",
        "input(age);",
        "output(\"Your Age Is:\");",
        "output(age);",
        "",
        "==================================================",
        "",
        "5. Basic Arithmetic: The Language Supports Basic Arithmetic That Is Calculated From Left To Right Without Precedence & Supports Only Addition, Subtraction, Multiplication & Division. Basic Arithmetic Only Works For The Following Data Types: INTEGER, DECIMAL & STRING.",
        "Basic Arithmetic Example:",
        "INTEGER int_sum = 100;",
        "int_sum = int_sum + 123 - 456 * 789 / 10;",
        "DECIMAL dec_sum = 123.456;",
        "dec_sum = dec_sum + 10.20 - 30.40 * 50.60 / 70.80;",
        "STRING string_concat = \"STRING 1\";",
        "string_concat = string_concat + \"STRING 2\" + \"STRING 3\";",
        "",
        "==================================================",
        "",
        "6. Control Flow: The Language Supports Control Flow Statements Which Are IF, ELSE IF & ELSE. It Also Supports N-Nested Control Flow Statements. Statements Can Handle Both Variables & Literals.",
        "Control Flow Example:",
        "Logical Operators: AND, OR, NOT",
        "Relational Operators: EQ (EQUALS), NE (NOT EQUAL), GT (GREATER THAN), LT (LESS THAN), GE (LESS THAN OR EQUAL TO), LE (GREATER THAN OR EQUAL TO)",
        "INTEGER int1 = 10;",
        "INTEGER int2 = 20;",
        "IF (1 EQ 1) OR (1 NE 2) OR (TRUE EQ TRUE) AND (TRUE NE FALSE) BRANCH;",
        "   output(\"BRANCH 1\");",
        "ELSE IF (int1 LT int2) AND (int2 GT int1) AND (0 GT 1) AND (1 LE 0) BRANCH;",
        "   output(\"BRANCH 2\");",
        "ELSE BRANCH;",
        "   output(\"BRANCH 3\");",
        "END;",
        "",
        "==================================================",
        "",
        "7. Arrays: The Language Supports Arrays Where The Arrays Must Be Declared & Assigned At The Same Time. The Supported Data Types Are The 5 Basic Variable Types. The 'ARRAY' Keyword Must Come Before The Data Type.",
        "Arrays Example:",
        "ARRAY INTEGER int_arr1 = [1,2,3,4,5];",
        "ARRAY DECIMAL dec_arr1 = [1.1,2.2,3.3,4.4,5.5];",
        "ARRAY CHARACTER char_arr1 = ['A','B','C','D','E'];",
        "ARRAY BOOLEAN bool_arr1 = [TRUE,FALSE,TRUE,FALSE,TRUE];",
        "ARRAY STRING str_arr1 = [\"STRING 1\",\"STRING 2\",\"STRING 3\",\"STRING 4\",\"STRING 5\"];",
        "",
        "==================================================",
        "",
        "8. Loops: The Language Supports For & While Loops.",
        "Loops Example:",
        "ARRAY INTEGER int_arr1 = [1,2,3,4,5];",
        "INTEGER arraySize = size(int_arr1);",
        "FOR LOOP (INTEGER index = 0; index LT arraySize; index++) BRANCH;",
        "   output(\"index\");",
        "   output(index);",
        "   INTEGER item;",
        "   item = int_arr1[index];",
        "   output(\"item:\");",
        "   output(item);",
        "FOR END;",
        "",
        "==================================================",
        "",
        "9. Functions: The Language Supports Functions Without Recursion Of The Same Function Call, But Can Support Nested Functions As Long As They Have Unique Varible Name Types Inside The Function Definition. Functions Have 6 Return Types Which Are VOID, INTEGER, DECIMAL, CHARACTER, BOOLEAN, STRING. Functions Can Support 0 Or Any Number Of Input Variables.",
        "Functions Example:",
        "RETURN_DATA_TYPE FUNCTION FUNCTION_NAME(INPUT PARAMETERS) BRANCH;",
        "...",
        "...",
        "...",
        "Code",
        "...",
        "...",
        "...",
        "FUNCTION END;",
        "VOID FUNCTION voidFunction() BRANCH;",
        "   output(\"VOID FUNCTION!\");",
        "   RETURN VOID;",
        "FUNCTION END;",
        "voidFunction();",
        "INTEGER FUNCTION intFunction(INTEGER int1) BRANCH;",
        "   output(\"INTEGER FUNCTION!\");",
        "   output(\"int1:\");",
        "   output(int1);",
        "   RETURN int1;",
        "   FUNCTION END;"
    ];
    let documentationText = "";
    for (let index = 0; index < codeDocumentationArray.length; index++) {
        documentationText += codeDocumentationArray[index];
        if (index + 1 < codeDocumentationArray.length) {
            documentationText += "\n";
        };
    };
    return documentationText;
};

/*
==================================================
The Execute Code Async Function Used To Send & Recieve Data To And From The Server
==================================================
*/
async function executeCode() {
    let clientCode = textEditor.value;
    let clientInput = terminalInput.value;
    console.log("[CLIENT] Execute Code! code:\n", clientCode);
    try {
        const result = await fetch("/execute", {
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
terminalInput.value = "27 , 155.5 , M , FALSE , CHRISTIAN TAMPUS";
documentation.placeholder = codeDocumentation();
documentation.value = codeDocumentation();
updateLineNumbers();


/*
==================================================
Program End
==================================================
*/
console.log("[CLIENT] script.js Program Terminated...");