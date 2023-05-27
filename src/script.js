document.getElementById("home").onclick = () => {
    window.location.href = "/";
}

let nav = document.getElementById("nav");
nav.childNodes.forEach((child) => {
    child.addEventListener("click", () => {
        child.innerHTML = child.id;
    })
})