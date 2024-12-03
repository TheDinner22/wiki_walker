var cy;

// just read the top box
function read_inputs(){
    in2 = document.getElementById("input2");
    in3 = document.getElementById("input3");
    in4 = document.getElementById("input4");
    return {
        "input2": in2.value,
        "input3": in3.value,
        "input4": in4.value
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
    cy = cytoscape({
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
            fit: true,
            name: 'fcose',
            nodeSeparation: 5000,
            idealEdgeLength: edge => 500,
            edgeElasticity: edge => 0.95,
            numIter: 25000
        }
    });
}

function reset_graph_colors(){
    cy.style()
        .selector('node')
        .style({
            'background-color': '#666',
        }).update()

    cy.style()
        .selector('edge')
        .style({
            'line-color': '#ccc',
            'target-arrow-color': '#ccc',
        }).update()
}

function color_node(node_id, color){
    cy.style()
        .selector('node[id="' + node_id +'"]')
        .style({
            'background-color': color
        }).update()
}

function color_edge(from, to, color){
    cy.style()
        .selector('edge[id="' +from+to+'"]')
        .style({
            'line-color': color,
            //'target-arrow-color': color,
        }).update()
}

function color_path(path, color){
    for (let index = 0; index < path.length-1; index++) {
        const from = path[index];
        const to = path[index+1];

        color_node(from, color)
        color_node(to, color)
        try {
            color_edge(from, to, color)
        } catch (e) { }
        try {
            color_edge(to, from, color)
        } catch (e) { }
    }
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

    // after every htmx swap (for times when we have html and json at same time
    document.body.addEventListener('htmx:afterSwap', function(event) {
        if(event.target.querySelectorAll("script").length == 1){
            const p = JSON.parse(event.target.querySelector("script").innerText);
            reset_graph_colors();
            color_path(p, 'red');
        }
    });

    //input_hint_setup("input1", ".input1_hint");
    input_hint_setup("input2", ".input2_hint");
    input_hint_setup("input3", ".input3_hint");
    input_hint_setup("input4", ".input4_hint");
})

