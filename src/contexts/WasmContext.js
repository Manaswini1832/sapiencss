import React, { createContext, useState, useEffect } from 'react';

export const WasmContext = createContext();

export const WasmProvider = ({ children }) => {
  const [wasmModule, setWasmModule] = useState(null);

  useEffect(() => {
    const loadWasm = () => {
      if (!wasmModule) {
        const script = document.createElement('script');
        script.src = '/wasm/sapienCSSCompiler.js';
        script.onload = () => {
          window.Module.onRuntimeInitialized = () => {
            setWasmModule(window.Module);
          };
        };
        document.body.appendChild(script);
      }
    };

    loadWasm();
  }, [wasmModule]);

  return (
    <WasmContext.Provider value={{ wasmModule }}>
      {children}
    </WasmContext.Provider>
  );
};
