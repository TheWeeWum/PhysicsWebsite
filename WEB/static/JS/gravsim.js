function refreshImages() {
  // Replace with your image IDs and sources
  document.getElementById("gravsimImage").src = "Images/gravsim.png?t=" + new Date().getTime();
}


document.addEventListener("DOMContentLoaded", function () {
  const form = document.getElementById("gravsimArgs");
  form.addEventListener("submit", function (event) {
    event.preventDefault(); // Prevent the form from submitting the traditional way

    // Collect form data
    const formData = new FormData(form);

    // Send form data using fetch
    fetch("/runGravsim", {
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
  var numMassesObj = document.getElementById("numMass");

  numMassesObj.addEventListener("input", function() {
    var numMasses = parseInt(numMassesObj.value);
    console.log(numMasses);

    // Clear previous additional inputs
    var additionalInputsDiv = document.getElementById("additionalInputs");
    additionalInputsDiv.innerHTML = '';

    // Create new input fields based on numMasses
    for (var i = 1; i <= numMasses; i++) {
        var div = document.createElement("div");
        div.classList.add("input-group-additional", "additional");
        div.innerHTML = '<label for="additional' + '">X for mass ' + i + ':</label>' +
                        '<input type="number" name="x' + i + '" id="x' + i + '">' +
                        '<label for="additional' + '">Y for mass ' + i + ':</label>' +
                        '<input type="number" name="y' + i + '" id="y' + i + '">' +
                        '<label for="additional' + '">Mass for mass ' + i + ':</label>' +
                        '<input type="number" name="mass' + i + '" id="mass' + i + '">';
          
        additionalInputsDiv.appendChild(div);
    }
  });
});