open SharedTypes;

let getClass = (gameState: gameState, field: field) =>
  switch (gameState) {
  | Winner(player) => field == Marked(player) ? "winner square" : "square"
  | _ => "square"
  };

let isFinished = (value: gameState) =>
  switch (value) {
  | Winner(_) => true
  | _ => false
  };

let toValue = (field: field) =>
  switch (field) {
  | Marked(Cross) => "X"
  | Marked(Circle) => "O"
  | Empty => ""
  };

[@react.component]
let make = (~value: field, ~gameState: gameState, ~onMark) => {
  <button
    className={getClass(gameState, value)}
    disabled={gameState |> isFinished}
    onClick={_evt => onMark()}>
    {value |> toValue |> React.string}
  </button>;
};
