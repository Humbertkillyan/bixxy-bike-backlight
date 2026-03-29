---
name: ✨ Feature Request
description: Proposer une nouvelle fonctionnalité
labels: ["enhancement"]

body:
  - type: markdown
    attributes:
      value: |
        Proposez une nouvelle feature pour BIXXY!

  - type: input
    id: title
    attributes:
      label: Titre de la feature
      placeholder: "[Module] Brève description"
    validations:
      required: true

  - type: textarea
    id: description
    attributes:
      label: Description
      description: Décrivez la feature proposée
      placeholder: "Expliquez ce que vous voulez ajouter..."
    validations:
      required: true

  - type: textarea
    id: use_case
    attributes:
      label: Cas d'usage
      description: Pourquoi cette feature est nécessaire?
      placeholder: "Décrivez le problème qu'elle résout..."
    validations:
      required: true

  - type: textarea
    id: implementation
    attributes:
      label: Approche suggérée
      description: Comment pensez-vous implémenter cela?
      placeholder: "Ideas sur l'architecture ou l'implémentation..."

  - type: checkboxes
    id: checklist
    attributes:
      label: Checklist
      options:
        - label: Cette feature suit les patterns BIXXY
        - label: Elle peut être modulaire et indépendante
        - label: J'ai considéré les impacts de performance
