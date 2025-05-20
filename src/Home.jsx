import { useContext, useState, useEffect } from "react";
import { WasmContext } from "./contexts/WasmContext";

export const Home = () => {
    const { wasmModule } = useContext(WasmContext);
    const [result, setResult] = useState("");
    const [input, setInput] = useState("MAKE RECTANGLE \"r1\" WITH color \"teal\", x \"10\", y \"100\", width \"500\", height \"100\";\nMAKE CIRCLE \"c1\" WITH color \"pink\", x \"50\", y \"50\", radius \"30\";\nMAKE LINE \"l1\" WITH color \"orange\", x \"1\", y \"15\", length \"1500\";\n"); // input state
    const [loading, setLoading] = useState(true);
    const [htmlString, setHtmlString] = useState('data:text/html,<html><body><canvas id="canvas" width="1500" height="1500"/></body></html>');

    useEffect(() => {
        if (wasmModule) {
            setLoading(false);
        }
    }, [wasmModule]);

    useEffect(() => {
        setHtmlString(prevHtml => {
            if (result.search("function draw") === -1) {
                // Clear the canvas
                return prevHtml + '<script>function draw(){const canvas = document.getElementById("canvas");if (canvas.getContext){const ctx = canvas.getContext("2d");ctx.clearRect(0, 0, canvas.width, canvas.height);}}draw();</script>';
            } else {
                // Draw on the canvas
                return prevHtml + "<script>" + result + "</script>";
            }
        });
    }, [result]);

    const handleClick = () => {   
        setHtmlString('data:text/html,<html><body><canvas id="canvas" width="1500" height="1500"/></body></html><script>function draw(){const canvas = document.getElementById("canvas");if (canvas.getContext){const ctx = canvas.getContext("2d");ctx.clearRect(0, 0, canvas.width, canvas.height);}}draw();</script>');
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
        <div>
            <p>Example : </p>
            <p>MAKE RECTANGLE "r1" WITH color "teal", x "10", y "100", width "500", height "100";</p>
            <p>MAKE CIRCLE "c1" WITH color "pink", x "50", y "50", radius "30";</p>
            <p>MAKE LINE "l1" WITH color "orange", x "1", y "15", length "1500";</p>
            <textarea
                rows="6"
                cols="60"
                placeholder="Enter your CSS-like code"
                value={input}
                onChange={(e) => setInput(e.target.value)}
            />
            <br />
            <button onClick={handleClick}>Compile</button>
            <h3>Compiler Output:</h3>
            <pre>{result}</pre>
            <iframe title="canvas-iframe" id="iframe" src={htmlString}></iframe>
        </div>
    );
};
