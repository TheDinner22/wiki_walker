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

        /*
        layout: {
            name: 'cise',
            fit: true,
            clusters: arrayOfClusterArrays,
            animate: false
        }
        */
    });

    let clusters = cy.elements().markovClustering({
        expandFactor: 2,        // affects time of computation and cluster granularity to some extent: M * M
        inflateFactor: 2,       // affects cluster granularity (the greater the value, the more clusters): M(i,j) / E(j)
        multFactor: 1,          // optional self loops for each node. Use a neutral value to improve cluster computations.
        maxIterations: 10
    });


    for(var i = 0; i<clusters.length; i++){
        for(var j = 0; j<clusters[i].length; j++){
            clusters[i][j]._private.data.clusterID = j % 20;
        }
    }


    let arrayOfClusterArrays = [];
    let uniqueClusterIDs = [];
    cy.nodes().forEach(function (node) {
        let clusterID = node.data('clusterID');
        console.log(clusterID)
        if (uniqueClusterIDs.includes(clusterID)) {
            if(arrayOfClusterArrays[clusterID] == undefined){
                arrayOfClusterArrays[clusterID] = [];
            }
            else{
                arrayOfClusterArrays[clusterID].push(node.data('id'));
            }
        }
        else {
            arrayOfClusterArrays.push([node.data('id')]);
        }
    });

    console.log(arrayOfClusterArrays) // the issue is that this ends up as an array of arrays or some bs

    cy.layout({
        name: 'cise',
        clusters: arrayOfClusterArrays,
        fit: true,
        nodeRepulsion: node => 4500,
        gravity: 0.25,
        gravityRange: 3.8,
        refresh: 1,

        maxIterations: 1000,        // Increase iterations for better precision
        nodeDistance: 40,           // Reduce distance between nodes
        edgeLength: 75,             // Fine-tune the edge length
        nodeOverlap: 50,            // Control how nodes overlap
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

