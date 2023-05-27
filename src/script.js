document.getElementById("home").onclick = () => {
    window.location.href = "/";
}

let nav = document.getElementById("nav");
nav.childNodes.forEach((child) => {
    child.addEventListener("click", () => {
        if (child.id == "types") {

        }
        else
            window.location.href = child.id + ".html";
    })
})