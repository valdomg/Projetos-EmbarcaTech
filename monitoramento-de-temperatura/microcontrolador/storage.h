#ifndef STORAGE_H
#define STORAGE_H

#include <Arduino.h>

// -----------------------------------------------------------------------------
// Estrutura de dados armazenada
// -----------------------------------------------------------------------------

/**
 * @struct ObjectStorage
 * @brief Estrutura que representa um registro armazenado no sistema.
 *
 * Cada objeto contém informações de temperatura, umidade
 * e um indicador de validade.
 */
struct ObjectStorage {
  float temperature = 0.0f; ///< Temperatura registrada em °C.
  float humidity    = 0.0f; ///< Umidade relativa registrada em %.
  bool valid        = false; ///< Indica se o objeto esta vazio.
};

// -----------------------------------------------------------------------------
// Funções da API de storage
// -----------------------------------------------------------------------------

/**
 * @brief Inicializa o sistema de armazenamento.
 *
 * - Monta o sistema de arquivos LittleFS.
 * - Essa funçao deve ser chamada no setup(). 
 */
void initStorage();

/**
 * @brief Salva um novo registro de temperatura e umidade no storage.
 *
 * Os dados são armazenados em formato JSON, cada registro em uma linha.
 *
 * @param temperature Valor de temperatura a salvar.
 * @param humidity    Valor de umidade a salvar.
 */
void saveStorage(float temperature, float humidity);

/**
 * @brief Obtém um registro do storage.
 *
 * - Lê o arquivo linha por linha.
 * - Retorna o objeto localizado no índice especificado.
 *
 * @param index Índice do objeto a recuperar (0 = primeiro elemento).
 * @return Estrutura ObjectStorage com os dados recuperados.
 *         Se o índice não for encontrado ou os dados forem inválidos,
 *         retorna um objeto com `valid = false`.
 */
ObjectStorage getObjectStorage(size_t index = 0);

/**
 * @brief Imprime todo o conteúdo do storage no Serial.
 *
 * - Lê o arquivo linha por linha.
 * - Desserializa e exibe cada objeto armazenado em formato legível.
 */
void storagePrint();

/**
 * @brief Remove o primeiro registro do storage.
 *
 * - Copia todos os registros, exceto o primeiro, para um arquivo temporário.
 * - Substitui o arquivo original pelo temporário.
 * 
 * 
 */
void deleteOneMessage();

/**
 * @brief Verifica se existem registros armazenados.
 *
 * @return true se há ao menos um registro no storage.
 * @return false caso contrário.
 */
bool hasDataStorage();

#endif // STORAGE_H
