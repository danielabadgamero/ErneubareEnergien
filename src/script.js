document.getElementById("home").onclick = () => {
	window.location.href = "/";
}

let nav = document.getElementById("nav");
nav.childNodes.forEach((child) => {
	child.addEventListener("click", () => {
		if (child.id != "types") {
			window.location.href = child.id + ".html";
		}
	})
})

let list = document.getElementById("arten-list");
list.childNodes.forEach((type) => {
	type.addEventListener("click", () => {
		window.location.href = type.id + ".html";
	});
});