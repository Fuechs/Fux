const button = document.querySelector('#white-mode')
const body = document.body;

button.addEventListener('click', function() {
    body.classList.toggle('white-mode');
})