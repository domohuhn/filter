
function openTab(evt, tab, makeInvis) {
    var i, tabcontent, tablinks;

    tabcontent = document.getElementsByClassName(makeInvis);
    for (i = 0; i < tabcontent.length; i++) {
        tabcontent[i].style.display = "none";
    }

    tablinks = document.getElementsByClassName("tablinks");
    for (i = 0; i < tablinks.length; i++) {
        tablinks[i].className = tablinks[i].className.replace(" active", "");
    }

    document.getElementById(tab).style.display = "block";
    evt.currentTarget.className += " active";
}

document.getElementsByName("defaultOpen").forEach((value,key,parent) => {value.click()});
