open Types;

type todoJS = Js.t {
    .
    active: bool,
    title: string
};
type todoInput = Js.t {
    .
    todo: todoJS
};
type mutation = Js.t {
    .
    variables: todoInput
};

type response = Js.t {.
    todos: list todo,
    loading : bool
};

type propsType = {
    data: response
};

let se = ReasonReact.stringToElement;

let lastTodoId = ref 0;

let component = ReasonReact.statefulComponent "Page";

let make ::todosQuery ::addTodoMutation ::toggleTodoMutation ::deleteTodoMutation _children => {
  ...component,
  render: fun _self => {
    let nonNullTodos = todosQuery##loading ? [] : todosQuery##todos;
  Js.log todosQuery##todos;
    let numOfItems = List.length nonNullTodos;
    <header className="header">
      <h1> (ReasonReact.stringToElement "Todos") </h1>
      <TodoInput
        addTodo=(
        fun _ => {
            let mutation = {
                "variables": {
                    "title": "test",
                    "active": Js.true_
                }
            };
            addTodoMutation mutation;
        }
        )
      />
    <TodoList
        todos=nonNullTodos
        toggleTodo=(
          fun id => toggleTodoMutation(id)
        )
        deleteTodo=(
          fun id => deleteTodoMutation(id)
        )
    />
      <div> (se ("Amount of todos: " ^ string_of_int numOfItems)) </div>
    </header>
  }
};

/*
 * Why are we exposing this JS interop, even though this file's only used by the
 * Reason side? Because the higher-order function returned by the `graphql` call
 * (wrapper, in dataTodoContainer) asks as an input the backing ReactJS class,
 * which Reason circumvents, and only exposes when some JS calls really needs
 * it. Remember that a ReasonReact "class" isn't a ReactJS class.
 */
let jsComponent = ReasonReact.wrapReasonForJs ::component (fun props => {
       make
       addTodoMutation::props##addTodoMutation
       todosQuery::props##todosQuery
       toggleTodoMutation::props##toggleTodoMutation
       deleteTodoMutation::props##deleteTodoMutation [||];
}

);
