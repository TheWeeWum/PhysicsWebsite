function refreshImages() {
  // Replace with your image IDs and sources
  document.getElementById("fluidImage").src = "Images/fluid.gif?t=" + new Date().getTime();
}

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