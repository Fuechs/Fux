const menu = document.querySelector('#mobile-menu')
const menuLinks = document.querySelector('.nav-menu')

menu.addEventListener('click', function() {
    menu.classList.toggle('is-active')
    menuLinks.classList.toggle('active')
})

menuLinks.addEventListener('click', function() {
    if (menu.classList.contains('is-active') && menuLinks.classList.contains('active')) {
        menu.classList.toggle('is-active')
        menuLinks.classList.toggle('active')
    }
})