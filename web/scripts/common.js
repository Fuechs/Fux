$(document).ready(function () {

    // feather icons
    feather.replace();

    // smooth scroll
    var scroll = new SmoothScroll('a[href*="#"]');

    // tiny slider
    $('#slider-1').slick({
        infinite: true,
        autoplay: true,
        autoplaySpeed: 3000,
        prevArrow: $('.prev'),
        nextArrow: $('.next'),
    });
});