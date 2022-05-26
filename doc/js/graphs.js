
var graphDictionary = {};



// Plots the frequency response graphs to the elements specified in
// the charts object.
//
// Each chart has a name and element id.
// charts.gain.name
// charts.gain.id
// charts.phase.name
// charts.phase.id
function plotPhaseAndGain(response, charts) {
    const configGain = {
        type: 'scatter',
        data: {
            datasets: [{
                label: 'Gain',
                backgroundColor: 'rgb(85, 170, 255)',
                borderColor: 'rgb(85, 170, 255)',
                data: [],
                showLine: true
            }]
        },
        options: {
            parsing: {
                xAxisKey: 'fs',
                yAxisKey: 'g'
            },
            scales: {
                x: {
                    title: {
                        display: true,
                        text: 'Frequency [Hz]'
                    }
                },
                y: {
                    title: {
                        display: true,
                        text: 'Gain'
                    }
                }
            }
        }
    };

    const configPhase = {
        type: 'scatter',
        data: {
            datasets: [{
                label: 'Phase',
                backgroundColor: 'rgb(85, 170, 255)',
                borderColor: 'rgb(85, 170, 255)',
                data: [],
                showLine: true
            }]
        },
        options: {
            parsing: {
                xAxisKey: 'fs',
                yAxisKey: 'p'
            },
            scales: {
                x: {
                    title: {
                        display: true,
                        text: 'Frequency [Hz]'
                    }
                },
                y: {
                    title: {
                        display: true,
                        text: 'Phase [\xB0]'
                    }
                }
            }
        }
    };

    for (let i = 0; i < response.size - 1; i++) {
        configGain.data.datasets[0].data.push({ fs: response.frequency(i), g: response.gain(i) });
        configPhase.data.datasets[0].data.push({ fs: response.frequency(i), p: response.phase(i) });
    }
    if(!(graphDictionary[charts.gain.name] === undefined)){
        graphDictionary[charts.gain.name].destroy()
    }

    graphDictionary[charts.gain.name] = new Chart(
        document.getElementById(charts.gain.id),
        configGain
    );

    
    if(!(graphDictionary[charts.phase.name] === undefined)){
        graphDictionary[charts.phase.name].destroy()
    }
    graphDictionary[charts.phase.name] = new Chart(
        document.getElementById(charts.phase.id),
        configPhase
    );
}

// Plots the response of the filter to a given input
// to the html canvas specified in the charts object.
//
// Each chart has a name and element id.
// chart.name
// chart.id
function plotResponse(response, chart) {
    const configPlot = {
        type: 'scatter',
        data: {
            datasets: [{
                label: 'Filter output',
                backgroundColor: 'rgb(85, 170, 255)',
                borderColor: 'rgb(85, 170, 255)',
                data: [],
                showLine: true
            },
            { 
            label: 'Filter input',
            backgroundColor: 'rgb(170, 85, 85)',
            borderColor: 'rgb(170, 85, 85)',
            data: [],
            showLine: true
            },
        ]
        },
        options: {
            parsing: {
                xAxisKey: 'x',
                yAxisKey: 'y'
            },
            scales: {
                x: {
                    title: {
                        display: true,
                        text: 'Time [s]'
                    }
                },
                y: {
                    title: {
                        display: true,
                        text: 'Signal (a.u.)'
                    }
                }
            }
        }
    };


    for (let i = 0; i < response.size - 1; i++) {
        configPlot.data.datasets[0].data.push({ x: response.x(i), y: response.output(i) });
        configPlot.data.datasets[1].data.push({ x: response.x(i), y: response.input(i) });
    }
    if(!(graphDictionary[chart.name] === undefined)){
        graphDictionary[chart.name].destroy()
    }

    graphDictionary[chart.name] = new Chart(
        document.getElementById(chart.id),
        configPlot
    );
}
