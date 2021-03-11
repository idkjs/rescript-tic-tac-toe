%%raw(`import './App.css';`)

@bs.module("./logo.svg") external logo: string = "default"

@react.component
let make = () => {
  <div className="App">
    <header className="App-header">
      <Main />
    </header>
  </div>
}
