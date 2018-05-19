# Learn ReasonML by building Tic Tac Toe in React

![](https://cdn-images-1.medium.com/max/2000/1*-ANKA_z3Mz_RFYKRjPnSNQ.jpeg)

Randomly, I am also a coding ex-lawyer. This is an update of [@coding_lawyer](https://twitter.com/coding_lawyer)'s excellent [article](https://medium.freecodecamp.org/learn-reasonml-by-building-tic-tac-toe-in-react-334203dd513c). If you have any question's better ask him though I will do my best to answer if you ask me. Basically dependencies and syntax have been updated to reason-react@0.4.1. Have fun!

You may have heard of [Reason](https://reasonml.github.io/) before. It’s a syntax on top of [OCaml](https://ocaml.org/) that compiles to both readable JavaScript code and to native and bytecode as well.

This means you could potentially write [a single application ](https://github.com/jaredly/gravitron)using Reason syntax, and be able to run it in the browser, and on Android and iOS phones as well.

This is one of the reasons why Reason (ouch, pun) is becoming increasingly popular. This is especially true in the JavaScript community because of the syntax similarities.

If you were a JavaScript developer before Reason came out and wanted to learn a functional programming (FP) language, you would have had to also learn a whole new syntax and set of rules. This might’ve discouraged many people.

With Reason, you mainly need to understand the FP principles on which it’s based — such as immutability, currying, composition, and higher-order-functions.

Before I discovered Reason, I was trying to use FP principles in JavaScript as much as I could. However, JavaScript is limited in this sense, since it’s not meant to be an FP language. To take advantage of these principles effectively, you need to use a bunch of libraries that create complicated abstractions which are hidden from you.

Reason, on the other hand, opens the entire FP realm to all interested JavaScript developers. It provides us with an opportunity to use all those cool OCaml features using syntax we dearly know.

Last but not least, we can write our [React](https://reasonml.github.io/reason-react/) or [React Native](https://github.com/reasonml-community/bs-react-native) apps using Reason.

## Why should you give Reason a try?

I hope you’ll discover the answer for yourself by the time you’ve finished reading this post.

As we go through the source code of the classic Tic Tac Toe game — written in Reason, using React — I’ll explain the core features of the language. You’ll see the benefits of the strong type system, immutability, pattern matching, functional composition using pipe, and so on. Unlike JavaScript, these features are intrinsic to Reason itself.

## Warming up

Before getting your hands dirty, you need to install Reason on your machine following [this guide](https://reasonml.github.io/docs/en/global-installation.html).

After that, you need to setup your app. To do this, you can either clone [my repository](https://github.com/codinglawyer/reason-tic-tac-toe) containing the code of our app or you can setup your own project using [ReasonScripts](https://github.com/reasonml-community/reason-scripts) and code along.

To view your app in the browser, you need to compile your Reason files to JavaScript ones first. The [BuckleScript](https://bucklescript.github.io/) compiler will take care of that.

In other words, when you run npm start (in the ReasonScripts project), your Reason code gets compiled to JavaScript. The result of the compilation is then rendered to the browser. You can see for yourself how readable the compiled code is by checking the lib folder inside your app.

## Our first component

![](https://cdn-images-1.medium.com/max/10368/1*rHPNoBsoXPI4GGG9rBZcPA.jpeg)

As we’ve already mentioned, our Tic Tac Toe app is written using [ReasonReact ](https://github.com/reasonml/reason-react)library. This makes Reason approachable for JavaScript developers, and a lot of newcomers are coming from this community.

Our app has a classic component structure, like any other React app. We’ll go through the components top-down when talking about UI, and bottom-up when describing their logic.

Let’s get started by taking a look at the top level App component.

    let component = ReasonReact.statelessComponent("App");
    let make = _children => {
      ...component,
      render: _self =>
        <div>
           <div className="title">
             (ReasonReact.string("Tic Tac Toe"))
           </div>
           <Game />
        </div>,
    };

The component gets created when you call ReasonReact.statelessComponent and pass the name of the component to it. You don’t need any class keywords like in React, since Reason doesn’t have any whatsoever.

The component is neither a class nor function — it’s a so-called [record](https://reasonml.github.io/docs/en/record.html). record is one of Reason’s data structures, which is similar to the JavaScript object. Unlike the latter, however, record is immutable.

Our new record component contains various default properties such as the initial state, lifecycle methods, and render. To adjust the component to our needs, we need to override some of these properties. We can do that inside the make function that returns our component.

Since the record is immutable, we can’t override its properties by mutation. Instead, we need to return a new record. To do this, we need to spread our component and redefine the properties we want to change. This is very similar to the JavaScript object spread operator.

Since the App is a pretty simple component, we want to override only the default render method so we can render our elements to the screen. The render method takes a single self argument that gives us access to the state and reducers, as we’ll see later.

Since ReasonReact supports [JSX](https://reactjs.org/docs/introducing-jsx.html), our render function can return JSX elements. The uncapitalized element will be recognized as a DOM element — div. The capitalized element will be recognized as a component — Game.

Due to Reason’s strong type system, you can’t simply pass a string to an element in order to display it, as you can in classic React.

Instead, you need to pass such string into a ReasonReact.string helper function that’ll convert it into reactElement which can be rendered.

Since this is a little bit verbose, and we’ll use this helper quite often, let’s store it in a toString variable. In Reason, you can use only the let keyword to do that.

    let toString = ReasonReact.string;

Before moving any further, let’s talk a bit about the make function’s arguments. Since we are not passing any props to the App component, it takes only the default children argument.

However, we are not using it. We can make this explicit by writing an underscore before it. If we haven’t done this, the compiler would give us a warning that the argument is not being used. We are doing the same with the self argument in the render method.

Understandable error and warning messages are another cool feature that’ll improve your developer experience, compared to JavaScript.

## Setting up variant types

![](https://cdn-images-1.medium.com/max/10776/1*d1Wy5Tj0ZfHWyqU2TXMiKg.jpeg)

Before diving into the application itself, we’ll define our types first.

Reason is a statically typed language. This means it evaluates the types of our values during the compilation time. In other words, you don’t need to run your app to check if your types are correct. This also means that your editor can provide you with [useful editing support](https://github.com/reasonml-editor/vscode-reasonml).

However, having a type system doesn’t mean you need to explicitly define types for all the values. If you decide not to, Reason will figure out (infer) the types for you.

We’ll take advantage of the type system to define the types that we’ll use throughout our app. This will force us to think about the structure of our app before coding it and we’ll get a code documentation as a bonus.

If you’ve had any experience with [TypeScript ](https://www.typescriptlang.org/)or [Flow](https://flow.org/), Reason types will look familiar. However, unlike these two libraries, you don’t need any previous configuration at all (I’m looking at you Typescript). Types are available out of the box.

In Reason, we can distinguish between [types](https://reasonml.github.io/docs/en/type.html) and [variant types](https://reasonml.github.io/docs/en/variant.html) (in short variants). Types are for example bool, string, and int. On the other hand, variants are more complex. Think of them as of enumerable sets of values—or more precisely, constructors. Variants can be processed via pattern matching, as we’ll see later.

    type player =
      | Cross
      | Circle;

    type field =
      | Empty
      | Marked(player);

Here we define player and field **variants**. When defining a variant, you need to use a type keyword.

Since we are building a Tic Tac Toe game, we’ll need two players. So, the player type will have two possible constructors — Cross and Circle.

If we think about the playing board, we know that each field type can have two possible constructors — either Empty or Marked by one of the players.

If you take a look at the Marked constructor, you can see that we are using it as a data structure. We use a variant to hold another piece of data. In our case, we pass it the player variant. This behavior is pretty powerful since it enables us to combine different variants and types together to create more complex types.

So, we’ve got the field variant. However, we need to define the whole playing board which consists of rows of fields.

    type row = list(field);
    type board = list(row);

Each row is a list of fields and the playing board is composed of a list of rows.

The list is one of Reason’s data structures—similar to the JavaScript array. The difference is, it’s immutable. Reason also has an array as a mutable fixed-length list. We’ll come back to these structures later.

    type gameState =
      | Playing(player)
      | Winner(player)
      | Draw;

Another variant we need to define is a gameState. The game can have three possible states. One of the players can be Playing, be a Winner, or we can have a Draw.

Now, we have all the types we need to compose the state of our game.

    type state = {
      board,
      gameState,
    };

Our component’s state is a record composed of the board and the gameState.

Before moving any further, I’d like to talk about modules. In Reason, files are modules. For example, we stored all our variants inside SharedTypes.re file. This code gets automatically wrapped inside the module like this:

    module SharedTypes {
      // variant types code
    }

If we wanted to access this module in a different file, we don’t need any import keyword. We can easily access our modules anywhere in our app using the dot notation — for example SharedTypes.gameState.

Since we are using our variants quite often, we can make it more concise by writing open SharedTypes at the top of the file in which we want to access our module. This allows us to drop the dot notation since we can use our module in the scope of our file.

## Establishing state

![](https://cdn-images-1.medium.com/max/8064/1*-uI3OZEzdeMzt6CGORmnYQ.jpeg)

Since we know how the state of our app will look, we can start building the game itself.

We’ve seen that our App component renders the Game component. This is the place where all the fun starts. I’ll walk you through the code step-by-step.

The App was a stateless component, similar to the functional component in React. On the other hand, the Game is a stateful one which means it can contain state and reducers. Reducers in Reason are based on the same principles as those you know from [Redux](https://github.com/reactjs/redux). You call an action, and the reducer will catch it and update the state accordingly.

To see what’s going on in the Game component, let’s inspect the make function (the code is shortened).

    let component = ReasonReact.reducerComponent("Game");

    let make = _children => {
      ...component,
      initialState: () => initialState,
      reducer: (action: action, state: state) => ...,
      render: ({state, send}) => ...,
    };

In the App component, we’ve overridden only the render method. Here, we are overriding reducer and initialState properties as well. We’ll talk about reducers later.

initialState is a function that (surprisingly) returns the initial state which we stored in a variable.

    let initialState = {
      board: [
        [Empty, Empty, Empty],
        [Empty, Empty, Empty],
        [Empty, Empty, Empty],
      ],
      gameState: Playing(Cross),
    };

If you scroll up a little bit and check our state type, you’ll see that the initialState has the same structure. It’s composed of the board that consists of rows of fields. At the beginning of the game all fields are Empty.

However, their status may change as the game goes on. Another part of the state is the gameState which is initially set to theCross player who plays first.

## Rendering board

Let’s take a look at the render method of our Game component.

      render: ({state, send}) =>
        <div className="game">
          <Board
            state
            onRestart=(_evt => send(Restart))
            onMark=(id => send(ClickSquare(id)))
          />
        </div>,

We already knew that it receives the self argument. Here, we use destructuring to access the state and the send function. This works just like in JavaScript.

The render method returns the Board component and passes it the state and two state handlers as props. The first one takes care of the app restart and the second one fires when the field gets marked by a player.

You might’ve noticed that we aren’t writing state=state when passing the state prop. In Reason, if we are not changing the prop’s name, we can pass prop using this simplified syntax.

Now, we can take a look at the Board component. I’ve omitted most of the render method for the time being.

    let component = ReasonReact.statelessComponent("Board");

    let make = (~state: state, ~onMark, ~onRestart, _children) => {
      ...component,
      render: (_) =>
        <div className="game-board">
          // ...
        </div>,
    };

The Board is a stateless component. As you might’ve noticed, the make function now takes several arguments. These are the props we’ve passed from the Game parent component.

The ~ symbol means that the argument is labeled. When calling a function with such an argument, we need to explicitly write the name of the argument when calling this function (component). And that’s what we did when we passed the props to it in the Game component.

You might’ve also noticed that we are doing another thing with one of the arguments — ~state:state. In the previous section, we defined our state type. Here, we are telling the compiler that the structure of this argument should be same as of the state type. You might know this pattern from Flow.

![](https://cdn-images-1.medium.com/max/11566/1*1KfeXO-IedHQ9cUI83cV3g.jpeg)

Let’s come back to the render method of the Board component.

Since we are dealing with lists there, we’ll talk about them a little bit more now, before inspecting the rest of the render method.

## Excursion I: list and array

In Reason, we have two data structures resembling JavaScript arrays — list and array. The list is immutable and resizable, whereas the array is mutable and has a fixed length. We are using a list due to its flexibility and efficiency which really shines when we use it recursively.

To map a list, you can use List.map method that receives two arguments—a function and a list. The function takes an element from the list and maps it. This works pretty much like the JavaScript Array.map. Here’s a simple example:

    let numbers = [1, 5, 8, 9, 15];
    let increasedNumbers = List.map((num) => num + 2, numbers);
    Js.log(increasedNumbers);  // [3,[7,[10,[11,[17,0]]]]]

What? You’re saying that the printed result looks weird? This is because the lists in Reason are [linked](https://en.wikipedia.org/wiki/Linked_list).

Printing lists in your code can be confusing. Fortunately, you can convert it into an array using the Array.of_list method.

    Js.log(Array.of_list(increasedNumbers));  // [3,7,10,11,17]

Let’s come back to our app and remind ourselves how our state looks.

    let initialState = {
      board: [
        [Empty, Empty, Empty],
        [Empty, Empty, Empty],
        [Empty, Empty, Empty],
      ],
      gameState: Playing(Cross),
    };

Inside the Board’s render method we first map over board which is composed of a list of rows. So, by mapping over it, we’ll gain access to the rows. Then, we render the BoardRow component.

    let component = ReasonReact.statelessComponent("Board");

    let make = (~state: state, ~onMark, ~onRestart, _children) => {
       ...component,
       render: (_) =>
          <div className="game-board">
             (
                ReasonReact.array(
                   Array.of_list(
                      List.mapi(
                        (index: int, row: row) =>
                         <BoardRow
                            key=(string_of_int(index))
                            gameState=state.gameState
                            row
                            onMark
                            index
                         />,
                       state.board,
                     ),
                 ),
               )
            )
         // ...

We are using the List.mapi method, which provides us with an index argument that we need to uniquely define our ids.

When mapping the list to the JSX elements, we need to do two additional things.

First, we need to convert it to an array using Array.of_list. Secondly, we need to convert the result to the reactElement using ReasonReact.array, since we (as already mentioned) can’t simply pass the string to the JSX element like in React.

To get to the field values, we need to map over each row as well. We are doing this inside the BoardRow component. Here, each element from the row is then mapped to the Square component.

    let component = ReasonReact.statelessComponent("BoardRow");

    let make = (~gameState: gameState, ~row: row, ~onMark, ~index: int, _children) => {
       ...component,
       render: (_) =>
          <div className="board-row">
             (ReasonReact.array(
                Array.of_list(
                   List.mapi(
                      (ind: int, value: field) => {
                        let id = string_of_int(index) ++ string_of_int(ind);
                        <Square
                           key=id
                           value
                           onMark=(() => onMark(id))
                           gameState
                        />;
                     },
                   row,
                 ),
              ),
            ))
        </div>,
    };

Using these two mappings, our board gets rendered. You’ll agree with me that the readability of this code isn’t so good because of all the function wrappings.

To improve it, we can use the pipe operator which takes our list data and pipes it through our functions. Here’s the second mapping example — this time using pipe.

    let component = ReasonReact.statelessComponent("BoardRow");

    let make = (~gameState: gameState, ~row: row, ~onMark, ~index: int, _children) => {
       ...component,
       render: (_) =>
          <div className="board-row">
             (
                row
                |> List.mapi((ind: int, value: field) => {
                   let id = string_of_int(index) ++ string_of_int(ind
                   <Square
                     key=id
                     value
                     onMark=(() => onMark(id))
                     gameState
                   />;
                 })
                |> Array.of_list
                |> ReasonReact.array
             )
          </div>,
    };

This makes our code much more readable, don’t you think? First, we take the row and pass it to the mapping method. Then, we convert our result to an array. Finally, we convert it to the reactElement.

By mapping our board, we are rendering a bunch of Square components to the screen and by doing so, we are creating the whole playing board.

We’re passing a couple of props to the Square. Since we want our id to be unique, we create it by combining indices from both mappings. We are also passing down the value which contains the field type that can be either Empty or Marked.

Finally, we pass a gameState and the onMark handler which will get invoked when a particular Square is clicked.

## Entering fields

![](https://cdn-images-1.medium.com/max/6000/1*8s-VzNIcca2gZ3JNLseODw.jpeg)

    let component = ReasonReact.statelessComponent("Square");

    let make = (~value: field, ~gameState: gameState, ~onMark, _children) => {
      ...component,
      render: _self =>
        <button
          className=(getClass(gameState, value))
          disabled=(gameState |> isFinished)
          onClick=(_evt => onMark())>
          (value |> toValue |> toString)
        </button>,
    };

The Square component renders a button and passes it some props. We are using a couple of helper functions here, but I won’t talk about all of them in detail. You can find them all in the [repo](https://github.com/idkjs/reason-tic-tac-toe).

The button’s class is calculated using the getClass helper function which turns the square green when one of the players wins. When this happens, all the Squares will be disabled as well.

To render the button’s value, we use two helpers.

    let toValue = (field: field) =>
      switch (field) {
      | Marked(Cross) => "X"
      | Marked(Circle) => "O"
      | Empty => ""
    };

toValue will convert the field type to the string using pattern matching. We’ll talk about pattern matching later. For now, you need to know that we are matching the field data to our three patterns. So, the result would be X, O, or an empty string. Then, we use toString to convert it to the reactElement.

Phew. We’ve just rendered the game board. Let’s quickly recap how we did it.

Our top-level App component renders the Game component which holds the game state and passes it down along with the handlers to the Board component.

The Board then takes the board state prop and maps the rows to the BoardRow component which maps the rows to the Square components. Each Square has an onClick handler that will fill it with a square or a circle.

## Make it do something already!

Let’s take a look at how our logic controlling the game works.

Since we have a board, we can allow a player to click on any square. When this happens, the onClick handler is fired and the onMark handler is called.

    // Square component
    <button
      className=(getClass(gameState, value))
      disabled=(gameState |> isFinished)
      onClick=(_evt => onMark())>
      (value |> toValue |> toString)
    </button>

The onMark handler got passed from the BoardRow component, but it was originally defined in the Game component that takes care of the state.

    // Game component
    render: ({state, send}) =>
        <div className="game">
          <Board
            state
            onRestart=(_evt => send(Restart))
            onMark=(id => send(ClickSquare(id)))
          />
        </div>,

We can see that the onMark prop is a ClickSquare reducer which we call using `self`'s send method, which means we are using it to update the state (as in Redux). The onRestart handler works similarly.

Notice that we are passing square’s unique id to the onMark handler inside the BoardRow component.

    // Board component
    (
      row
      |> List.mapi((ind: int, value: field) => {
        let id = string_of_int(index) ++ string_of_int(ind
        <Square
          key=id
          value
          onMark=(() => onMark(id))
          gameState
        />;
       })
      |> Array.of_list
      |> ReasonReact.array
    )

Before taking a look at our reducers in detail, we need to define actions to which our reducers will respond.

    type action =
      | ClickSquare(string)
      | Restart;

As with the global variant types, this forces us to think about our logic before we start implementing it. We define two action variants. ClickSquare takes one argument that will have a type of astring.

Now, let’s take a look at our reducers.

    let updateBoard = (board: board, gameState: gameState, id) =>
      board
      |> List.mapi((ind: int, row: row) =>
        row
          |> List.mapi((index: int, value: field) =>
            string_of_int(ind) ++ string_of_int(index) === id ?
              switch (gameState, value) {
              | (*_*, Marked(*_*)) => value
              | (Playing(player), Empty) => Marked(player)
              | (*_*, Empty) => Empty
              } :
              value
          )
      );

    reducer: (action: action, state: state) =>
        switch (action) {
        | Restart => ReasonReact.Update(initialState)
        | ClickSquare((id: string)) =>
           let updatedBoard = updateBoard(state.board, state.gameState, id);
           ReasonReact.Update({
             board: updatedBoard,
             gameState:
                checkGameState3x3(updatedBoard, state.board, state.gameState),
           });
        },

The ClickSquare reducer takes an id of the particular Square. As we’ve seen, we are passing in the BoardRow component. Then, our reducer calculates a new state.

For the board state update, we’ll call the updateBoard function. It uses the same mapping logic we used in the Board and BoardRow component. Inside of it, we map over the state.board to get the rows and then map over the rows to get the field values.

Since the id of each square is a composition of ids from both mappings, we’ll use it to find the field which the player clicked. When we find it, we’ll use the pattern matching to determine what to do with it. Otherwise, we’ll leave the square’s value unmodified.

## Excursion II: pattern matching

![](https://cdn-images-1.medium.com/max/6000/1*ZqEe4lxV-aAJUEy5b6MKfQ.jpeg)

We use the pattern matching to process our data. We define **patterns **which we’ll match against our **data**. When exercising the pattern matching in Reason, we use a switch statement.

    switch (state.gameState, value) {
      | (*_*, Marked(*_*)) => value
      | (Playing(player), Empty) => Marked(player)
      | (*_*, Empty) => Empty
    }

In our case, we are using a [tuple](https://reasonml.github.io/docs/en/tuple.html) to represent our **data**. Tuples are data structures that separate data with commas. Our tuple contains the gameState and the value (containing the field type).

Then we define multiple **patterns** that we’ll match against our data. The first match determines the result of the entire pattern matching.

By writing an underscore inside the pattern, we are telling the compiler that we don’t care what the particular value is. In other words, we want to have a match every time.

For example, the first pattern is matched when the value is Marked by any player. So, we don’t care about the gameState and we don’t care about the player type either.

When this pattern is matched, the result is the original value. This pattern prevents players from overriding already marked Squares.

The second pattern addresses the situation when any player is playing, and the field is Empty. Here, we use the player type in the pattern and then again in the result. We are basically saying that we don’t care about which player is playing (Circle or Cross) but we still want to mark the square according to the player that is actually playing.

The last pattern acts as the default one. If the first or the second pattern isn’t matched, the third will always match. Here, we don’t care about the gameState.

However, since we’re checking for the Playing game state in the previous pattern, we are now checking for the Draw or Winner gameState type. If this is the case, we’ll leave the field Empty. This default scenario prevents players from continuing to play when the game is over.

A cool thing about pattern matching in Reason is that the compiler will warn you if you haven’t covered all the possible pattern matches. This will save you a lot of trouble, because you’ll always know if you’ve covered all the possible scenarios. So, if the compiler is not giving you any warnings, your pattern matching will never fail.

When the pattern matching is finished, the particular field gets updated. When all the mappings are done, we get a new board state and store it as the updatedBoard. We can then update the component’s state by calling ReasonReact.Update.

    ReasonReact.Update({
      board: updatedBoard,
      gameState:
        checkGameState3x3(updatedBoard, state.board, state.gameState),

We update the board state using the result of the pattern matching. When updating the gameState, we call the checkGameState3x3 helper which calculates the state of the game for us.

## Do we have a winner?

![](https://cdn-images-1.medium.com/max/11232/1*GP9tKFzB_XqNkHSWuiuk1A.jpeg)

Let’s take a look what the checkGameState3x3 does.

First, we need to define all the possible combinations of winning fields (for the 3x3 board) and store them as winningCombs. We also have to define the winningRows type.

    type winningRows = list(list(int));

    let winningCombs = [
      [0, 1, 2],
      [3, 4, 5],
      [6, 7, 8],
      [0, 3, 6],  
      [1, 4, 7],
      [2, 5, 8],
      [0, 4, 8],
      [2, 4, 6],
    ];

We passed this list to the checkGameState function as the first argument.

    let checkGameState3x3 = checkGameState(winningCombs);

By doing this, we are taking advantage of the [currying](https://en.wikipedia.org/wiki/Currying) principle. When we pass the winningCombs to the checkGameState function, we get back a new function waiting for the rest of the arguments to be passed. We store this new function as the checkGameState3x3.

This behavior is really helpful, since we are able to configure the checkGameState function depending on the width and height of the board.

Let’s see what’s going on inside the checkGameState function.

    let checkGameState =
      (
        winningRows: winningRows,
        updatedBoard: board,
        oldBoard: board,
        gameState: gameState,
      ) =>
     oldBoard == updatedBoard ?
       gameState :
       {
         let flattenBoard = List.flatten(updatedBoard);
         let rec check = (rest: winningRows) => {
           let head = List.hd(rest);
           let tail = List.tl(rest);
           switch (
             getWinner(flattenBoard, head),
             gameEnded(flattenBoard),
             tail,
           ) {
           | (Cross, *_*, *_*) => Winner(Cross)
           | (Circle, *_*, *_*) => Winner(Circle)
           | (*_*, true, []) => Draw
           | (*_*, false, []) => whosPlaying(gameState)
           | *_* => check(tail)
           };
        };
        check(winningRows);
    };

First, we check if the board state is different from the previous one. If that’s not the case, we’ll return the unchanged gameState. Otherwise, we’ll calculate the new game state.

### Calculating new states

![](https://cdn-images-1.medium.com/max/6528/1*x2gUD6BhNjkF7ac4UEwg5Q.jpeg)

We start determining our new game state by converting the board part of the state, which consists of a list of rows, to a simple list using List.flatten. The flattened result will have this kind of structure:

    [Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty]

Back in the function, we define a check function that receives a single rest argument that has type of winningRows . The rec keyword before its definition means that it can be invoked recursively. However, for the recursive function calls, we need recursive data as well. Fortunately, the list is a recursive data structure.

We’ve already learned that lists in Reason are linked. This feature enables us to iterate through [lists using recursion](http://reasonmlhub.com/exploring-reasonml/ch_recursion.html) easily.

At the bottom of the checkGameState, we call the check function for the first time and pass it the winningCombs list. Inside the function, we extract the first element from the list and store it as the head. The rest of the list gets stored as the tail.

After that, we use the pattern matching again. We already know how it works, so I won’t go into detail. But it’s worth checking how we define our data and patterns.

    type winner =
      | Cross
      | Circle
      | NoOne;

    switch (
      getWinner(flattenBoard, head),
      gameEnded(flattenBoard),
      tail,
    ) { ...

Inside the switch statement, we use a tuple again to represent our data. Our tuple contains three elements—winner type as a result of the getWinner function, boolean as a result of the gameEnded function, and remaining list elements (tail).

Before going any further, let’s talk a bit about these two helper functions.

We’ll take a look inside the getWinner function first.

    let getWinner = (flattenBoard, coords) =>
      switch (
        List.nth(flattenBoard, List.nth(coords, 0)),
        List.nth(flattenBoard, List.nth(coords, 1)),
        List.nth(flattenBoard, List.nth(coords, 2)),
      ) {
      | (Marked(Cross), Marked(Cross), Marked(Cross)) => Cross
      | (Marked(Circle), Marked(Circle), Marked(Circle)) => Circle
      | (*_*, *_*, *_*) => NoOne
      };

When we call the check recursive function for the first time, the head will be the first element of the winningRows, that is [0, 1, 2] which is a list. We pass head to the getWinner function as the coords argument along with the flattenBoard.

Again, we use the pattern matching with the tuple. Inside the tuple, we use the List.nth method to access the equivalent positions of the coords coordinates in the flattened board list. The List.nth function takes a list and a number and returns the list’s element to that position.

So, our tuple consists of the three winning coordinates of our board that we’ve accessed using List.nth.

Now, we can match our tuple data against the patterns. The first two patterns check if all three fields are marked by the same player. If they are, we’ll return the winner — Cross or Circle. Otherwise, we’ll return NoOne.

Let’s see what’s going on inside the gameEnded function. It checks if all the fields are Marked and returns a boolean.

    let gameEnded = board =>
      List.for_all(
        field => field == Marked(Circle) || field == Marked(Cross),
        board,
      );

Since we know what values can be returned from our helper functions, let’s come back to our check function.

    switch (
      getWinner(flattenBoard, head),
      gameEnded(flattenBoard),
      tail,
      ) {
      | (Cross, *_*, *_*) => Winner(Cross)
      | (Circle, *_*, *_*) => Winner(Circle)
      | (*_*, true, []) => Draw
      | (*_*, false, []) => whosPlaying(gameState)
      | *_* => check(tail)
      };

Our pattern matching can now determine if the game ended in a win or draw. If these cases are not matched, we’ll move to the following case.

If it is matched, the game will continue and the whosPlaying function will be called, and the other player will take a turn.

If these cases are not matched, we’ll move to the following case. If it is matched, the game will continue and the whosPlaying function will be called, and the other player will take a turn.

    let whosPlaying = (gameState: gameState) =>
      switch (gameState) {
      | Playing(Cross) => Playing(Circle)
      | *_* => Playing(Cross)
      };

Otherwise, we’ll call the check function recursively with a new combination of winning fields.

That’s it. Now you know how our code controlling the game logic works.

## That’s all folks!

I hope this post helped you to understand the core features of this promising and still-developing language. However, to fully appreciate the power of this new syntax on top of OCaml, you need to start building your own stuff. Now you’re ready to do that.

Good luck!

![](https://cdn-images-1.medium.com/max/11360/1*X1DG2-9Eusuj5c1IyOTc0g.jpeg)

If you liked this article, give it a few claps**.** I would greatly appreciate it and more people will be able to see this post as well.

If you have any questions, criticism, observations, or tips for improvement, feel free to write a comment below or reach the original author via [Twitter](https://twitter.com/coding_lawyer) or [his blog](https://codinglawyer.net/).

Thanks to [mediumexporter](https://github.com/xdamman/mediumexporter) by [@xdamman](https://twitter.com/xdamman)and [markdowntomedium](http://markdowntomedium.com/) by [@jacobbennet](https://twitter.com/JacobBennett).
