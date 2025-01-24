// @ts-ignore
import { fetch } from '@tauri-apps/plugin-http';
// @ts-ignore
import { storage,CardModule } from './main';
export class Main implements CardModule {
    public activate:boolean=true;
    public module_name:string="reviewer";
    public initialize_html:string=`
<button class="w3-button w3-round-large" id="reviewerButtonClearBuffer">clear buffer</button>
<div id="reviewerIndex"></div>
<div id="reviewerPage"></div>`;
    private main_url:string="https://initiatoratao.github.io/modules/index.html";
    private current_url:string=this.main_url;
    private current_file:string="";

    private async fetch_file(url:string) {
        console.log("fetching: "+url);
        const cachedPages=storage.getItem(this.module_name,url);
        if(cachedPages)
            return cachedPages;
        else {
            console.log("cache miss!");
            const text:string=await fetch(url,{
                method: "GET",
            }).then(response=>{return response.text();});
            storage.setItem(this.module_name,url,text);
            return text;
        }
    }

    private async update_index(url:URL) {
        const index=document.querySelector("#reviewerIndex");
        if(index) {
            index.innerHTML=await this.fetch_file(url.href);
            const links=index.querySelectorAll("a");
            links.forEach(link => {
                link.onclick=(event:MouseEvent) => {
                    event.preventDefault();
                    const link=event.target as HTMLAnchorElement;
                    if(link) {
                        const href=link.getAttribute("href");
                        if(href) {
                            const url:URL=new URL(this.current_url.replace("index.html","")+href);
                            if(href.endsWith("/index.html")) {
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
        this.current_url=url.href;
    }

    private async update_file(url:URL) {
        const page=document.querySelector("#reviewerPage");
        this.current_file=await this.fetch_file(url.href);
        if(page) {
            page.innerHTML=this.current_file; 
            (window as any).MathJax.typeset();
        }
    }

    public clearBuffer() {
        storage.clearModuleItems(this.module_name);
        this.update_index(new URL(this.main_url));
    }
    public initialize_func() {
        this.update_index(new URL(this.main_url));
        const button:HTMLButtonElement|null=document.querySelector("#reviewerButtonClearBuffer");
        if(button) button.onclick=() => {
            this.clearBuffer();
        };
    }
}
