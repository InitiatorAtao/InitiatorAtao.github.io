// @ts-ignore
import { fetch } from '@tauri-apps/plugin-http';
// @ts-ignore
import { storage } from './main';
export class Main {
    constructor() {
        this.activate = true;
        this.module_name = "reviewer";
        this.initialize_html = `
<button class="w3-button w3-round-large" id="reviewerButtonClearBuffer">clear buffer</button>
<div id="reviewerIndex"></div>
<div id="reviewerPage"></div>`;
        this.main_url = "https://initiatoratao.github.io/modules/index.html";
        this.current_url = this.main_url;
        this.current_file = "";
    }
    async fetch_file(url) {
        console.log("fetching: " + url);
        const cachedPages = storage.getItem(this.module_name, url);
        if (cachedPages)
            return cachedPages;
        else {
            console.log("cache miss!");
            const text = await fetch(url, {
                method: "GET",
            }).then(response => { return response.text(); });
            storage.setItem(this.module_name, url, text);
            return text;
        }
    }
    async update_index(url) {
        const index = document.querySelector("#reviewerIndex");
        if (index) {
            index.innerHTML = await this.fetch_file(url.href);
            const links = index.querySelectorAll("a");
            links.forEach(link => {
                link.onclick = (event) => {
                    event.preventDefault();
                    const link = event.target;
                    if (link) {
                        const href = link.getAttribute("href");
                        if (href) {
                            const url = new URL(this.current_url.replace("index.html", "") + href);
                            if (href.endsWith("/index.html")) {
                                this.update_index(url);
                            }
                            else {
                                this.update_file(url);
                            }
                        }
                    }
                };
            });
        }
        this.current_url = url.href;
    }
    async update_file(url) {
        const page = document.querySelector("#reviewerPage");
        this.current_file = await this.fetch_file(url.href);
        if (page) {
            page.innerHTML = this.current_file;
            window.MathJax.typeset();
        }
    }
    clearBuffer() {
        storage.clearModuleItems(this.module_name);
        this.update_index(new URL(this.main_url));
    }
    initialize_func() {
        this.update_index(new URL(this.main_url));
        const button = document.querySelector("#reviewerButtonClearBuffer");
        if (button)
            button.onclick = () => {
                this.clearBuffer();
            };
    }
}
