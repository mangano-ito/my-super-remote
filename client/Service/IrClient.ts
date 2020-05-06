import Preset from '#/Model/Preset';
import Agent from './Agent';

type PathBuilder = (...params: any) => string;
type PathBuilderMap = {[key in Endpoints]: PathBuilder};

/**
 * Endpoints
 */
enum Endpoints {
    LIST,
    EMIT,
}

/**
 * Endpoint Path Builder Map
 */
const ENDPOINT_PATH_BUILDERS: PathBuilderMap = {
    [Endpoints.LIST]: () => '/preset',
    [Endpoints.EMIT]: (name: string) => `/preset/${name}`,
};

/**
 * Ir-API Client
 */
export default class IrClient {
    private agent: Agent;

    /**
     * @param root server root
     */
    constructor(root = 'http://127.0.0.1:8000') {
        this.agent = new Agent(root);
    }

    async record() {
        // not implemented
    }

    async list(): Promise<Preset[]> {
        const path = ENDPOINT_PATH_BUILDERS[Endpoints.LIST]();

        return this.agent.get(path);
    }

    /**
     * emit preset sequence
     * @param preset preset to emit
     */
    async emit_by_preset_name(name: string): Promise<any> {
        const path = ENDPOINT_PATH_BUILDERS[Endpoints.EMIT](name);

        return this.agent.post(path);
    }
}
