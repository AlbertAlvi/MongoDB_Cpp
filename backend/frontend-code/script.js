"use strict";

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

	fetch("http://localhost:8080/submit", {
		method: "POST",
		headers: {
			"Content-Type": "application/json",
		},
		body: JSON.stringify(formData),
	})
		.then((response) => {
			console.log("Response:", response);
		})
		.catch((error) => {
			console.error("Error:", error);
		});
}

const form_div = document.querySelector("#userForm");
const data_div = document.querySelector("#data");

const data_btn = document.querySelector("#data-btn");
const form_btn = document.querySelector("#form-btn");

data_btn.addEventListener("click", () => {
	fetch("http://localhost:8080/data", {
		method: "GET",
	})
		.then((response) => response.json())
		.then((arr_json) => {
			// console.log("Json:", json);
			displayData(arr_json);
		})
		.catch((error) => {
			console.error("Error:", error);
		});

	form_div.style.display = "none";
	data_div.style.display = "flex";
});
form_btn.addEventListener("click", () => {
	data_div.style.display = "none";
	form_div.style.display = "block";
});

function displayData(arr_json) {
	data_div.innerHTML = "";
	for (let obj of arr_json) {
		const item = document.createElement("div");
		item.classList.add("item");
		item.innerHTML = `
		<span class="material-symbols-outlined">delete</span>${JSON.stringify(obj)}
		`;
		data_div.appendChild(item);
		
		// storing _id for deleting
		const span = item.querySelector("span");
		span.setAttribute("data-id", `${obj["_id"]["$oid"]}`);
		span.addEventListener("click", (evt)=>{
			console.log(evt.target);
		});
	}
}
