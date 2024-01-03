// Function to toggle dark mode
function toggleDarkMode() {
    const body = document.body;
    body.classList.toggle("dark-mode");
    const isDarkMode = body.classList.contains("dark-mode");
    setThemeCookie(isDarkMode);
}

// Function to set the theme cookie
function setThemeCookie(isDarkMode) {
    const theme = isDarkMode ? "dark" : "light";
    document.cookie = `theme=${theme};expires=Fri, 31 Dec 9999 23:59:59 GMT;path=/`;
}

// Check for theme preference in the cookie and apply it on page load
window.onload = function () {
    const theme = getCookie("theme");
    if (theme === "dark") {
        document.body.classList.add("dark-mode");
    }
}

// Function to get a specific cookie by name
function getCookie(name) {
    const value = `; ${document.cookie}`;
    const parts = value.split(`; ${name}=`);
    if (parts.length === 2) return parts.pop().split(';').shift();
}
