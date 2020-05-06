export default class Agent {
    /**
     * @param root server root
     */
    constructor(private root: string) {}

    async get<T = any>(path: string): Promise<T> {
        const url = `${this.root}${path}`;

        return fetch(url).then((res) => res.json());
    }

    async post<T = any>(path: string): Promise<T> {
        const url = `${this.root}${path}`;

        return fetch(url, { method: 'POST' }).then((res) => res.json());
    }
}
