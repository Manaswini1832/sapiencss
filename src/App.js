import React from 'react';
import { WasmProvider } from './contexts/WasmContext';
import { Home } from './Home';

function App() {

  return (
    <WasmProvider>
    <div className="App">
      <Home/>
    </div>
    </WasmProvider>
  );
}

export default App;
