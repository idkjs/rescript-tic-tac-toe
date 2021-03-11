open SharedTypes

@react.component
let make = (~gameState: gameState, ~row: row, ~onMark, ~index: int) =>
  <div className="board-row">
    {row
    |> List.mapi((ind: int, value: field) => {
      let id = string_of_int(index) ++ string_of_int(ind)
      <Square key=id value onMark={() => onMark(id)} gameState />
    })
    |> Array.of_list
    |> React.array}
  </div>
