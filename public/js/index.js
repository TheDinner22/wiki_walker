function read_inputs(){
    in2 = document.getElementById("input2");
    return {
        "input2": in2.value
    }
}

// returns a function that hides or un-hides the element with the given class name
function hide(class_name){ return () => { document.querySelector(class_name).classList.add("hidden"); } }
function un_hide(class_name){ return () => { document.querySelector(class_name).classList.remove("hidden"); } }

// li_got_clicked
function li_got_clicked(e){
    console.log("hi")
    content = e.target.innerText;
    console.log(content)

}

function input_hint_setup(input_id, hint_class_name){
    // setup focus event listener on input
    input1 = document.getElementById(input_id);
    input1.addEventListener("focus", un_hide(hint_class_name));
    input1.addEventListener("focusout", hide(hint_class_name));
}



// this is the same as main in js land
document.addEventListener('DOMContentLoaded', () => {
    //input_hint_setup("input1", ".input1_hint");
    input_hint_setup("input2", ".input2_hint");
})

