// just read the top box
function read_inputs(){
    in2 = document.getElementById("input2");
    return {
        "input2": in2.value
    }
}

// read bottom two boxes
function read_start_end(){
    in3 = document.getElementById("input3");
    in4 = document.getElementById("input4");
    return {
        "input3": in3.value,
        "input4": in4.value,
    }
}

// returns a function that hides or un-hides the element with the given class name
function hide(class_name){ return () => { document.querySelector(class_name).classList.add("hidden"); } }
function un_hide(class_name){ return () => { document.querySelector(class_name).classList.remove("hidden"); } }

function li_got_clicked(event, textbox_id){
    // just set the textbox with given id to have the value
    document.getElementById(textbox_id).value = event.target.innerText;
}

function input_hint_setup(input_id, hint_class_name){
    // setup focus event listener on input
    input1 = document.getElementById(input_id);
    input1.addEventListener("focus", un_hide(hint_class_name));
    input1.addEventListener("focusout", hide(hint_class_name));
}

function graph_tests(){
    var cy = cytoscape({
        container: document.querySelector('.tree-container'), // container to render in
        elements: [ // list of graph elements to start with
            { // node a
                data: { id: 'a' }
            },
            { // node b
                data: { id: 'b' }
            },
            { // edge ab
                data: { id: 'ab', source: 'a', target: 'b' }
            }
        ],

        style: [ // the stylesheet for the graph
            {
                selector: 'node',
                    style: {
                        'background-color': '#666',
                            'label': 'data(id)'
                    }
            },

            {
                selector: 'edge',
                style: {
                    'width': 3,
                    'line-color': '#ccc',
                    'target-arrow-color': '#ccc',
                    'target-arrow-shape': 'triangle',
                    'curve-style': 'bezier'
                }
            }
        ],

        layout: {
            name: 'grid',
            rows: 1
        }
    });
}

/*
    
    {
        endpoint: function name
        nodes: list of nodes
        edges: edge list [ [to from], [to from], [to, from]]

    }

    * */
function make_graph(data){
    // first create a nodes list
    let elements = [];
    for (let i = 0; i < data.nodes.length; i++) {
        elements.push({ data: {id: data.nodes[i]}});
    }

    // next add edges
    for (let i = 0; i < data.edges.length; i++) {
        elements.push({
            data: { id: data.edges[i][0]+data.edges[i][1], source: data.edges[i][0], target: data.edges[i][1] }
        });
    }
    
    // and we should be able to see a graph
    var cy = cytoscape({
        container: document.querySelector('.tree-container'), // container to render in
        elements: elements,

        style: [ // the stylesheet for the graph
            {
                selector: 'node',
                    style: {
                        'background-color': '#666',
                            'label': 'data(id)'
                    }
            },

            {
                selector: 'edge',
                style: {
                    'width': 3,
                    'line-color': '#ccc',
                    'target-arrow-color': '#ccc',
                    'target-arrow-shape': 'triangle',
                    'curve-style': 'bezier'
                }
            }
        ],

        layout: {
            name: 'grid',
            rows: 100,
            columns: 100
        }
    });

}

// this is the same as main in js land
document.addEventListener('DOMContentLoaded', () => {
    // after every htmx:request :(
    document.body.addEventListener('htmx:afterRequest', function(event) {
        const response = event.detail.xhr.responseText;
        try {
            const data = JSON.parse(response);
            if (data.endpoint === 'make_graph') {
                make_graph(data);
            }
        } catch (e) {}

    });

    //input_hint_setup("input1", ".input1_hint");
    input_hint_setup("input2", ".input2_hint");
    input_hint_setup("input3", ".input3_hint");
    input_hint_setup("input4", ".input4_hint");

    // graph_tests();
})

