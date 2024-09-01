function refreshImages() {
  // Replace with your image IDs and sources
  document.getElementById("fluidImage").src = "Images/fluid.gif?t=" + new Date().getTime();
}

var sliderX = document.getElementById("x");
var outputX = document.getElementById("xout");
var sliderY = document.getElementById("y");
var outputY = document.getElementById("yout");
var sliderFrames = document.getElementById("frames");
var outputFrames = document.getElementById("framesOut");
var sliderPhysicsFrames = document.getElementById("physicsPerGraphic");
var outputPhysicsFrames = document.getElementById("physicsPerGraphicOut");
var sliderDT = document.getElementById("dt");
var outputDT = document.getElementById("dtOut");   

var sliderEffectRadius = document.getElementById("effectRadius");
var outputEffectRadius = document.getElementById("effectRadiusOut");

var sliderGravx = document.getElementById("gravx");
var outputGravx = document.getElementById("gravxOut");
var sliderGravy = document.getElementById("gravy");
var outputGravy = document.getElementById("gravyOut");

var sliderNumParticles = document.getElementById("numParticles");
var outputNumParticles = document.getElementById("numParticlesOut");

var sliderMinMass = document.getElementById("minMass");
var outputMinMass = document.getElementById("minMassOut");
var sliderMaxMass = document.getElementById("maxMass");
var outputMaxMass = document.getElementById("maxMassOut");

var sliderMinX = document.getElementById("minX");
var outputMinX = document.getElementById("minXOut");
var sliderMaxX = document.getElementById("maxX");
var outputMaxX = document.getElementById("maxXOut");

var sliderMinY = document.getElementById("minY");
var outputMinY = document.getElementById("minYOut");
var sliderMaxY = document.getElementById("maxY");
var outputMaxY = document.getElementById("maxYOut");

var sliderMinVX = document.getElementById("minVX");
var outputMinVX = document.getElementById("minVXOut");
var sliderMaxVX = document.getElementById("maxVX");
var outputMaxVX = document.getElementById("maxVXOut");

var sliderMinVY = document.getElementById("minVY");
var outputMinVY = document.getElementById("minVYOut");
var sliderMaxVY = document.getElementById("maxVY");
var outputMaxVY = document.getElementById("maxVYOut");

var sliderViewPressure = document.getElementById("pressureView");
var outputViewPressure = document.getElementById("pressureViewOut");
var sliderViewParticles = document.getElementById("particleView");
var outputViewParticles = document.getElementById("particleViewOut");

var sliderPressureConstant = document.getElementById("pressureConstant");
var outputPressureConstant = document.getElementById("pressureConstantOut");
var sliderViscosity = document.getElementById("viscosity");
var outputViscosity = document.getElementById("viscosityOut");


var sliders = [sliderX, sliderY, sliderFrames, sliderPhysicsFrames, sliderDT, sliderEffectRadius, sliderGravx, sliderGravy, sliderNumParticles, sliderMinMass, sliderMaxMass, sliderMinX, sliderMaxX, sliderMinY, sliderMaxY, sliderMinVX, sliderMaxVX, sliderMinVY, sliderMaxVY, sliderViewParticles, sliderViewPressure, sliderPressureConstant, sliderViscosity];
var outputs = [outputX, outputY, outputFrames, outputPhysicsFrames, outputDT, outputEffectRadius, outputGravx, outputGravy, outputNumParticles, outputMinMass, outputMaxMass, outputMinX, outputMaxX, outputMinY, outputMaxY, outputMinVX, outputMaxVX, outputMinVY, outputMaxVY, outputViewParticles, outputViewPressure, outputPressureConstant, outputViscosity];

for (var i = 0; i < sliders.length; i++) {
  outputs[i].value = sliders[i].value;
  outputs[i].setAttribute("max", sliders[i].getAttribute("max"));
  outputs[i].setAttribute("min", sliders[i].getAttribute("min"));
  outputs[i].setAttribute("step", sliders[i].getAttribute("step"));
}

function setUpperLimit(slider, output, containValue1, containValue2=null) {
  if (containValue2 != null) {
    slider.setAttribute("max", Math.min(containValue1, containValue2));
    output.setAttribute("max", Math.min(containValue1, containValue2));
  }
  else {
    slider.setAttribute("max", containValue1);
    output.setAttribute("max", containValue1);
  }

  if (parseInt(slider.getAttribute("max").value) < parseInt(slider.value)) {
    slider.value = slider.getAttribute("max").value;
  }
  output.value = slider.value;
}

for (let i = 0; i < sliders.length; i++) {
  sliders[i].oninput = function() {
    outputs[i].value = sliders[i].value;
  }
}

for (let i = 0; i < sliders.length; i++) {
  outputs[i].oninput = function() {
    sliders[i].value = outputs[i].value;
  }
}

sliderX.oninput = function() {
  outputX.value = sliderX.value;
  setUpperLimit(sliderEffectRadius, outputEffectRadius, outputX.value, outputY.value);
  sliderMinX.setAttribute("max", parseInt(outputX.value) - 3);
  sliderMaxX.setAttribute("max", parseInt(outputX.value) - 3);
}

outputX.oninput = function() {
  sliderX.value = outputX.value;
  setUpperLimit(sliderEffectRadius, outputEffectRadius, outputX.value, outputY.value);
  sliderMinX.setAttribute("max", parseInt(outputX.value) - 3);
  sliderMaxX.setAttribute("max", parseInt(outputX.value) - 3);
}
sliderY.oninput = function() {
  outputY.value = sliderY.value;
  setUpperLimit(sliderEffectRadius, outputEffectRadius, outputX.value, outputY.value);
  sliderMinY.setAttribute("max", parseInt(outputY.value) - 3);
  sliderMaxY.setAttribute("max", parseInt(outputY.value) - 3);
}
outputY.oninput = function() {
  sliderY.value = outputY.value;
  setUpperLimit(sliderEffectRadius, outputEffectRadius, outputX.value, outputY.value);
  sliderMinY.setAttribute("max", parseInt(outputY.value) - 3);
  sliderMaxY.setAttribute("max", parseInt(outputY.value) - 3);
}

function moveSmaller(max, minSlider, minOutput) {
  if (parseInt(max.value) < parseInt(minSlider.value)) {
    minSlider.value = max.value;
    minOutput.value = max.value;
  }
}
function moveLarger(min, maxSlider, maxOutput) {
  if (parseInt(min.value) > parseInt(maxSlider.value)) {
    maxSlider.value = min.value;
    maxOutput.value = min.value;
  }
}

// mass oninputs
sliderMinMass.oninput = function() {
  outputMinMass.value = sliderMinMass.value;
  moveLarger(sliderMinMass, sliderMaxMass, outputMaxMass);
}
outputMinMass.oninput = function() {
  sliderMinMass.value = sliderMinMass.value;
  moveLarger(sliderMinMass, sliderMaxMass, outputMaxMass);
}
sliderMaxMass.oninput = function() {
  outputMaxMass.value = sliderMaxMass.value;
  moveSmaller(sliderMaxMass, sliderMinMass, outputMinMass);
}
outputMaxMass.oninput = function() {
  sliderMaxMass.value = sliderMaxMass.value;
  moveSmaller(sliderMaxMass, sliderMinMass, outputMinMass);
}


// bounding box oninputs
sliderMinX.oninput = function() {
  outputMinX.value = sliderMinX.value;
  moveLarger(sliderMinX, sliderMaxX, outputMaxX);
}
outputMinX.oninput = function() {
  sliderMinX.value = outputMinX.value;
  moveLarger(sliderMinX, sliderMaxX, outputMaxX);
}

sliderMaxX.oninput = function() {
  outputMaxX.value = sliderMaxX.value;
  moveSmaller(sliderMaxX, sliderMinX, outputMinX);
}
outputMaxX.oninput = function() {
  sliderMaxX.value = outputMaxX.value;
  moveSmaller(sliderMaxX, sliderMinX, outputMinX);
}

sliderMinY.oninput = function() {
  outputMinY.value = sliderMinY.value;
  moveLarger(sliderMinY, sliderMaxY, outputMaxY);
}
outputMinY.oninput = function() {
  sliderMinY.value = outputMinY.value;
  moveLarger(sliderMinY, sliderMaxY, outputMaxY);
}

sliderMaxY.oninput = function() {
  outputMaxY.value = sliderMaxY.value;
  moveSmaller(sliderMaxY, sliderMinY, outputMinY);
}
outputMaxY.oninput = function() {
  sliderMaxY.value = outputMaxY.value;
  moveSmaller(sliderMaxY, sliderMinY, outputMinY);
}

// velocity bounding box oninputs
sliderMinVX.oninput = function() {
  outputMinVX.value = sliderMinVX.value;
  moveLarger(sliderMinVX, sliderMaxVX, outputMaxVX);
}
outputMinVX.oninput = function() {
  sliderMinVX.value = outputMinVX.value;
  moveLarger(sliderMinVX, sliderMaxVX, outputMaxVX);
}
sliderMaxVX.oninput = function() {
  outputMaxVX.value = sliderMaxVX.value;
  moveSmaller(sliderMaxVX, sliderMinVX, outputMinVX);
}
outputMaxVX.oninput = function() {
  sliderMaxVX.value = outputMaxVX.value;
  moveSmaller(sliderMaxVX, sliderMinVX, outputMinVX);
}

sliderMinVY.oninput = function() {
  outputMinVY.value = sliderMinVY.value;
  moveLarger(sliderMinVY, sliderMaxVY, outputMaxVY);
}
outputMinVY.oninput = function() {
  sliderMinVY.value = outputMinVY.value;
  moveLarger(sliderMinVY, sliderMaxVY, outputMaxVY);
}
sliderMaxVY.oninput = function() {
  outputMaxVY.value = sliderMaxVY.value;
  moveSmaller(sliderMaxVY, sliderMinVY, outputMinVY);
}
outputMaxVY.oninput = function() {
  sliderMaxVY.value = outputMaxVY.value;
  moveSmaller(sliderMaxVY, sliderMinVY, outputMinVY);
}

sliderEffectRadius.getAttribute("max").value = Math.min(outputX.value, outputY.value);


document.addEventListener("DOMContentLoaded", function () {
  const form = document.getElementById("fluidArgs");
  form.addEventListener("submit", function (event) {
    event.preventDefault(); // Prevent the form from submitting the traditional way
    
    // Collect form data
    const formData = new FormData(form);

    // Send form data using fetch
    fetch("/runFluid", {
      method: "POST",
      body: formData,
    })
    .then((response) => {
      if (response.ok) {
        return response.text();
      } else {
        throw new Error("Failed to run Python script.");
      }
    })
    .then((data) => {
      console.log("Success:", data);
      console.log("refreshing");
      refreshImages();
      // Handle success, update UI or show message
      // alert('Form submission successful!');
    })
    .catch((error) => {
      console.error("Error:", error);
      // Handle error
      // alert('Form submission failed!');
    });
  });
});


// JavaScript code to handle additional input fields based on Z input
document.addEventListener("DOMContentLoaded", function() {
});