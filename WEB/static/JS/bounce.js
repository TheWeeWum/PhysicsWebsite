function refreshImages() {
  // Replace with your image IDs and sources
  document.getElementById("bounceImage").src = "Images/bounce.gif?t=" + new Date().getTime();
}


document.addEventListener("DOMContentLoaded", function () {
  const form = document.getElementById("bounceArgs");
  form.addEventListener("submit", function (event) {
    event.preventDefault(); // Prevent the form from submitting the traditional way

    // Collect form data
    const formData = new FormData(form);

    // Send form data using fetch
    fetch("/runBounce", {
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
  var numCirclesObj = document.getElementById("numCircles");
  var numPointsObj = document.getElementById("numPoints");
  var numSpringsObj = document.getElementById("numSprings");

  numCirclesObj.addEventListener("input", function() {
    var numCircles = parseInt(numCirclesObj.value);

    // Clear previous additional inputs
    var additionalInputsDiv = document.getElementById("C_additionalInputs");
    additionalInputsDiv.innerHTML = '';

    // Create new input fields based on numCircles
    for (var i = 1; i <= numCircles; i++) {
        var div = document.createElement("div");
        div.classList.add("input-group-additional", "additional");
        div.innerHTML = '<label for="additional' + '">Number of Points in the Circle ' + i + ':</label>' +
                        '<input type="number" name="C_numPoints' + i + '" id="C_numPoints' + i + '">' +
                        
                        '<label for="additional' + '">X Coord for Center of Circle ' + i + ':</label>' +
                        '<input type="number" name="C_xCenter' + i + '" id="C_xCenter' + i + '">' +

                        '<label for="additional' + '">Y Coord for Center of Circle ' + i + ':</label>' +
                        '<input type="number" name="C_yCenter' + i + '" id="C_yCenter' + i + '">' +

                        '<label for="additional' + '">Circle Radius ' + i + ':</label>' +
                        '<input type="number" name="C_radius' + i + '" id="C_radius' + i + '">' +

                        '<label for="additional' + '">Mass for each point in Cirlce ' + i + ':</label>' +
                        '<input type="number" name="C_mass' + i + '" id="C_mass' + i + '">' +
                        
                        '<label for="additional' + '">Spring Constant ' + i + ':</label>' +
                        '<input type="number" name="C_springConstant' + i + '" id="C_springConstant' + i + '">' +
                        
                        '<label for="additional' + '">Equilibrium ratio between points on the circle (1 means they start at equilibrium, 2 means they want to double their distance) ' + i + ':</label>' +
                        '<input type="number" name="C_equilib' + i + '" id="C_equilib' + i + '">' +
                        
                        '<label for="additional' + '">Initial X Velocity of Cirlce ' + i + ':</label>' +
                        '<input type="number" name="C_vx' + i + '" id="C_vx' + i + '">' +
                        
                        '<label for="additional' + '">Initial Y Velocity of Cirlce ' + i + ':</label>' +
                        '<input type="number" name="C_vy' + i + '" id="C_vy' + i + '">';
          
        additionalInputsDiv.appendChild(div);
    }
  });

  numPointsObj.addEventListener("input", function() {
    var numPoints = parseInt(numPointsObj.value);

    // Clear previous additional inputs
    var additionalInputsDiv = document.getElementById("P_additionalInputs");
    additionalInputsDiv.innerHTML = '';

    // Create new input fields based on numPoints
    for (var i = 1; i <= numPoints; i++) {
        var div = document.createElement("div");
        div.classList.add("input-group-additional", "additional");
        div.innerHTML ='<label for="additional' + '">Mass ' + i + ':</label>' +
                        '<input type="number" name="mass' + i + '" id="C_mass' + i + '">' +
                        
                        '<label for="additional' + '">X position ' + i + ':</label>' +
                        '<input type="number" name="x' + i + '" id="P_x' + i + '">' +

                        '<label for="additional' + '">Y position ' + i + ':</label>' +
                        '<input type="number" name="y' + i + '" id="P_y' + i + '">' +

                        '<label for="additional' + '">Initial X Velocity ' + i + ':</label>' +
                        '<input type="number" name="vx' + i + '" id="P_vx' + i + '">' +
                        
                        '<label for="additional' + '">Initial Y Velocity ' + i + ':</label>' +
                        '<input type="number" name="vy' + i + '" id="P_vy' + i + '">'
                        
                        '<label for="additional' + '">Number of Spring Connections ' + i + ':</label>' +
                        '<input type="number" name="num connections' + i + '" id="P_numConnections' + i + '">';
        
        additionalInputsDiv.appendChild(div);
    }
  });

  numSpringsObj.addEventListener("input", function() {
    var numSprings = parseInt(numSpringsObj.value);

    // Clear previous additional inputs
    var additionalInputsDiv = document.getElementById("S_additionalInputs");
    additionalInputsDiv.innerHTML = '';

    // Create new input fields based on numSprings
    for (var i = 1; i <= numSprings; i++) {
        var div = document.createElement("div");
        div.classList.add("input-group-additional", "additional");
        div.innerHTML ='<label class="additional" for="additional' + '">Equilibrium Distance ' + i + ':</label>' +
                        '<input type="number" name="equilibrium' + i + '" id="S_equilib' + i + '">' + 

                        '<label class="additional" for="additional' + '">Springs Constant ' + i + ':</label>' +
                        '<input type="number" name="spring const' + i + '" id="S_springConst' + i + '">' + 
                        
                        '<label class="additional" for="additional' + '">First point to attach to ' + i + ':</label>' +
                        '<input type="number" name="(index of the point)' + i + '" id="S_index1' + i + '">' + 
                        
                        '<label class="additional" for="additional' + '">Second point to attach to ' + i + ':</label>' +
                        '<input type="number" name="(index of the point)' + i + '" id="S_index2' + i + '">';
        additionalInputsDiv.appendChild(div);
    }
  });
});