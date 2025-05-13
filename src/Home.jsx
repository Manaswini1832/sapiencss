import { useContext, useState, useEffect } from "react";
import { WasmContext } from "./contexts/WasmContext";

export const Home = () => {
    const { wasmModule } = useContext(WasmContext);
    const [result, setResult] = useState("");
    const [input, setInput] = useState(""); // input state
    const [loading, setLoading] = useState(true);

    useEffect(() => {
        if (wasmModule) {
            setLoading(false);
        }
    }, [wasmModule]);

    useEffect(() => {
        if(result){
            if(result.search("function draw") === -1){
                let clearCanvas = new Function('function draw(){const canvas = document.getElementById("canvas");if (canvas.getContext){const ctx = canvas.getContext("2d");ctx.clearRect(0, 0, canvas.width, canvas.height);}}draw();');
                clearCanvas();
                alert(result);
            }else{
                try {
                    let drawSapCSSCanvas = new Function(result);
                    drawSapCSSCanvas();   
                } catch (error) {
                    alert(error);
                }
            }
        }
    }, [result]);

    const handleClick = () => {   
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
            <canvas id="canvas" width="1500" height="1500"/>
        </div>
    );
};
