import IrClient from '#/Service/IrClient';

const singletonApi = new IrClient();

export function provideSingletonApi() { 
    return singletonApi;
}
