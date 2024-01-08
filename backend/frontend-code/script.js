"use strict"

function submitForm() {
    let name = document.getElementById("name").value;
    let age = document.getElementById("age").value;
    let role = document.getElementById("role").value;
    let department = document.getElementById("department").value;
  
    let formData = {
      name: name,
      age: parseInt(age),
      role: role,
      department: department,
    };

    // console.log('Form Data:', formData);
  
    fetch('http://localhost:8080/submit', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify(formData),
    })
    .then(response => {
      console.log('Success:', response);
    })
    .catch(error => {
      console.error('Error:', error);
    });
  }
  