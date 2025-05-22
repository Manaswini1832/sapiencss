import { useContext, useState, useEffect } from "react";
import { WasmContext } from "./contexts/WasmContext";
import "./Home.css";

export const Home = () => {
    const { wasmModule } = useContext(WasmContext);
    const [result, setResult] = useState("");
    const [input, setInput] = useState("#Remove comments and click Compile\nMAKE RECTANGLE \"r1\" WITH color \"teal\", x \"10\", y \"111\", width \"80\", height \"100\";\nMAKE CIRCLE \"c1\" WITH color \"gold\", x \"50\", y \"60.5\", radius \"50\";\nMAKE CIRCLE \"c2\" WITH color \"black\", x \"30\", y \"50\", radius \"5\";\nMAKE CIRCLE \"c3\" WITH color \"black\", x \"70\", y \"50\", radius \"5\";\nMAKE CIRCLE \"c3\" WITH color \"coral\", x \"50\", y \"80\", radius \"15\";\n#MAKE LINE \"l1\" WITH color \"orange\", x \"1\", y \"15\", length \"1500\";\n"); // input state
    const [loading, setLoading] = useState(true);
    const [htmlString, setHtmlString] = useState('data:text/html,<html><body><canvas id="canvas" width="1500" height="1500"/></body></html>');
    const [error, setError] = useState(false);
    const startScriptTag = '<script>';
    const endScriptTag = '</script>';

    useEffect(() => {
        if (wasmModule) {
            setLoading(false);
        }
    }, [wasmModule]);

    useEffect(() => {
        setHtmlString(prevHtml => {
            if (result.search("function draw") === -1) {
                // Clear the canvas
                setError(true);
                return prevHtml + startScriptTag + 'function draw(){const canvas = document.getElementById("canvas");if (canvas.getContext){const ctx = canvas.getContext("2d");ctx.clearRect(0, 0, canvas.width, canvas.height);}}draw();' + endScriptTag;
            } else {
                // Draw on the canvas
                setError(false);
                return prevHtml + startScriptTag + result + endScriptTag;
            }
        });
    }, [result]);

    const handleClick = () => {   
        //setHtmlString('data:text/html,<html><body><canvas id="canvas" width="1500" height="1500"/></body></html><script>function draw(){const canvas = document.getElementById("canvas");if (canvas.getContext){const ctx = canvas.getContext("2d");ctx.clearRect(0, 0, canvas.width, canvas.height);}}draw();</script>');
        if (input && wasmModule) {
            const compileInput = input.endsWith('\n') ? input : input + '\n';

            const sapienCSSCompiler = wasmModule.cwrap("sapienCSSCompiler", "number", ["string"]);
            const outputPtr = sapienCSSCompiler(compileInput);

            const output = wasmModule.UTF8ToString(outputPtr);

            setResult(output);

            const freeResult = wasmModule.cwrap("freeResult", null, ["number"]);
            freeResult(outputPtr);
        }
    };

    return loading ? (
        <div>Loading...</div>
    ) : (
        <div className="container">
            <div className="editor">
                <textarea
                    className="editor_textarea"
                    rows="6"
                    cols="60"
                    placeholder="Enter your CSS-like code"
                    value={input}
                    onChange={(e) => setInput(e.target.value)}
                />
                <br />
                <button onClick={handleClick}>Compile</button>
            </div>
            {error ? <pre>{result}</pre> : null}
            <div className="result">
                <iframe title="canvas-iframe" id="iframe" src={htmlString}></iframe>
            </div>
        </div>
    );
};
