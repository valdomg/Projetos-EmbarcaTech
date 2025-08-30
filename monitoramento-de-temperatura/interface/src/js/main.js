

var gauge = new JustGage({
  id: "gauge", // the id of the html element
  value: 50,
  min: 0,
  max: 100,
  decimals: 0,
  symbol: '°c',
  gaugeWidthScale: 0.9,
  title: "Lone Ranger",
  label: "Última",
  // label: "Temperatura",
  pointer: "true",
  pointerOptions: {
    toplength: 0,
    bottomlength: 15,
    bottomwidth: 3,
    stroke: 'none',
    stroke_width: 0,
    stroke_linecap: 'square',
    color: 'dark'
  }
});

// update the value randomly
setInterval(() => {
  gauge.refresh(Math.random() * 100);
}, 5000)

var gauge2 = new JustGage({
  id: "gauge2", // the id of the html element
  value: 50,
  min: 0,
  max: 100,
  decimals: 0,
  gaugeWidthScale: 0.9,
  symbol: '%',
  // label: "Umidade",
  pointer: "true",
  pointerOptions: {
    toplength: 0,
    bottomlength: 15,
    bottomwidth: 3,
    stroke: 'none',
    stroke_width: 0,
    stroke_linecap: 'square',
    color: 'dark'
  },
  levelColors: ["#9bd4faff", "#008cffff", "#0400ffff"]
});

// update the value randomly
setInterval(() => {
  gauge2.refresh(Math.random() * 100);
}, 5000)