"use strict";

const form_div = document.querySelector("#employee-form");
const submit_btn = form_div.querySelector("#submit-btn");

submit_btn.addEventListener("click", submitForm);

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

	fetch("http://localhost:8080/submit", {
		method: "POST",
		headers: {
			"Content-Type": "application/json",
		},
		body: JSON.stringify(formData),
	})
		.then((response) => {
			console.log("Response:", response);
			// Limpar os campos do formulário
            form_div.name.value = "";
            form_div.age.value = "";
            form_div.role.value = "";
            form_div.department.value = "";
		})
		.catch((error) => {
			console.error("Error:", error);
		});
}

const data_div = document.querySelector("#employee-data");

const data_btn = document.querySelector("#data-btn");
const form_btn = document.querySelector("#form-btn");

data_btn.addEventListener("click", fetchData);
form_btn.addEventListener("click", () => {
	data_div.style.display = "none";
	form_div.style.display = "block";
});

function fetchData() {
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
}

function displayData(arr_json) {
	data_div.innerHTML = "";
	for (let obj of arr_json) {
		const item = document.createElement("div");
		item.classList.add("item");
		item.innerHTML = `
		<span class="material-symbols-outlined">delete</span>
		<div>
			Name: ${obj["name"]} | Age: ${obj["age"]} | Role: ${obj["role"]} | Department: ${obj["department"]}
		</div>
		`;
		data_div.appendChild(item);

		// storing _id for deleting
		const span = item.querySelector("span");
		span.setAttribute("data-id", `${obj["_id"]["$oid"]}`);
		span.addEventListener("click", (evt) => {
			const _id = evt.target.getAttribute("data-id");
			fetch("http://localhost:8080/remove", {
				method: "POST",
				headers: {
					"Content-Type": "application/json",
				},
				body: JSON.stringify({ _id }),
			})
				.then((response) => {
					console.log("remove:", response);
					fetchData();
				})
				.catch((error) => {
					console.error(error);
				});
		});
	}
}
