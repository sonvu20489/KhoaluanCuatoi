document.addEventListener("DOMContentLoaded", ()=>{
    var btn_toggle = document.querySelector('.symbol_menu');
    var menu = document.querySelector('.menu');
    var toggle = 0;
    btn_toggle.addEventListener("click",()=>{
        if(toggle == 0)
        {
            document.querySelector(".symbol_menu").innerText = '╳';
            document.querySelector(".symbol_menu").setAttribute('style','font-size:18px');
            menu.setAttribute('style','display:flex;');
            toggle = !toggle;
        }
        else
        {
            document.querySelector(".symbol_menu").setAttribute('style','');
            document.querySelector(".symbol_menu").innerText = '≡';
            menu.setAttribute("style",'display:none');
            toggle = !toggle;
        }
    })
})